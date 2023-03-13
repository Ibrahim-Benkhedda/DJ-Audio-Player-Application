#include "PlaylistComponent.h"

PlaylistComponent::PlaylistComponent(juce::AudioFormatManager& formatManagerToUse)
    : formatManagerToUse(formatManagerToUse), juce::XmlElement("Playlist") {  

    otherLookAndFeel = std::make_unique<OtherLookAndFeel>();
       
    addTables();
    addTextBoxes();
    addButtons();    
    tableComponent.setModel(this);
      
    // Currently there's no track selected
    currentIndex = -1;
    // Searchbar is not used
    usingSearch = false;
}

PlaylistComponent::~PlaylistComponent() {
    // prevents memory leak
    tableComponent.setModel(nullptr);
    loadButton.setLookAndFeel(nullptr);
    eraseButton.setLookAndFeel(nullptr);
    importButton.setLookAndFeel(nullptr);
    exportButton.setLookAndFeel(nullptr);
    searchBar.setLookAndFeel(nullptr);
}

void PlaylistComponent::paint (juce::Graphics& g) {
    g.fillAll (juce::Colour::fromRGB(40, 40, 40));   // clear the background
    g.setColour (juce::Colour::fromRGB(30, 0, 70));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    g.setColour(juce::Colour::fromRGB(30, 0, 70));
    g.fillRect(getLocalBounds());
}

void PlaylistComponent::resized() {
    int row = getWidth() / 4;
    searchBar.setBounds(0, 0, getWidth(), 25);
    tableComponent.setBounds(0, 25, getWidth(), getHeight() - 25);       
    loadButton.setBounds(0, getHeight() - 24, row, 24);
    eraseButton.setBounds(row, getHeight() - 24, row, 24);
    importButton.setBounds(row * 2, getHeight() - 24, row, 24);
    exportButton.setBounds(row * 3, getHeight() - 24, row, 24);
}

int PlaylistComponent::getNumRows() {
    if (usingSearch) {
        return static_cast<int>(searchResultIndices.size());
    }
    else {
        return static_cast<int>(tracks.size());
    }  
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
                                            int rowNumber,
                                            int width,
                                            int height,
                                            bool rowIsSelected ) {
    if (rowIsSelected) {
        g.fillAll(juce::Colour::fromRGB(30, 0, 70));
    }
    else {
        g.fillAll(juce::Colour::fromRGB(30, 30, 30));
    }
}

void PlaylistComponent::paintCell(
    juce::Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected ) {

    Track& track = getTrackForRow(rowNumber);

    g.setColour(juce::Colours::white);

    // If the row number is less than the number of rows
    if (rowNumber < getNumRows()) {
        // track title column
        if (columnId == 1) {
            g.drawText(track.name,
                2, 0, width - 4, height,
                juce::Justification::centredLeft,
                true
            );         
        }
        // track format/extension column
        else if (columnId == 2) {
            g.drawText(track.extension,
                2, 0, width - 4, height,
                juce::Justification::centredLeft,
                true
            );          
        }
        // track duration column
        else if (columnId == 3) {
            g.drawText(track.duration,
                2, 0, width - 4, height,
                juce::Justification::centredLeft,
                true
            ); 
        }
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                            int columnId,
                                            bool isRowSelected,
                                            juce::Component* existingComponentToUpdate) {
    
    Track& track = tracks[rowNumber];

    if (columnId == 4) {
        // creates the deck 1 button  if it's null
        if (existingComponentToUpdate == nullptr) {
            existingComponentToUpdate = createCellButton("DECK 1", rowNumber);
        }
    }
    else if (columnId == 5) {
        // creates the deck 2 button if it's null
        if (existingComponentToUpdate == nullptr) {
            existingComponentToUpdate = createCellButton("DECK 2", rowNumber);
        }
    }
    else if (columnId == 6 ) {
        // creates delete button if it's null
        if (existingComponentToUpdate == nullptr) {
            existingComponentToUpdate = createCellButton("DELETE", rowNumber);
        }   
    }

    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button) {
    if (button == &loadButton) {
        loadTracks();
    }
    else if (button == &exportButton) {
        exportPlaylist();
    }
    else if (button == &importButton) {
        importPlaylist();   
    }
    else if (button == &eraseButton) {
        tracks.clear();
        searchResultIndices.clear();
        tableComponent.updateContent();
        repaint();
    }
    else {
        // convert juce string to int 
        int id = std::stoi(button->getComponentID().toStdString());
    
        const Track& track = usingSearch ? tracks[searchResultIndices[id]] : tracks[id];

        if (button->getName() == "DECK 1") {
            DBG("LoadToDeck1 is playing");
            loadToDeckCallBack(track.path, 1);

        }
        else if (button->getName() == "DECK 2") {
            DBG("LoadToDeck2 is playing");
            loadToDeckCallBack(track.path, 2);
        }
        else if (button->getName() == "DELETE") {
            if (usingSearch) {
                deleteSelectedTrack(id, true);
            }
            else {             
                deleteSelectedTrack(id, false);
            }

            tableComponent.updateContent();
            repaint();
        }
    }         
}

bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray& files) {
    return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y) {
    for (juce::String filename : files) {
        juce::File trackFile{ filename };
        // Use the audio format reader to get the file's length and sample rate
        // use we unique_ptr instead of raw pointer to prevent memory leak
        std::unique_ptr<juce::AudioFormatReader> reader(formatManagerToUse.createReaderFor(trackFile));
        Track track = track.getMetaData(std::move(reader), trackFile);
        tracks.push_back(track);
    }

    for (Track track : tracks) {
        DBG(track.name + track.extension + " -> " + track.artist + " : " + track.path);
    }
    // update the table so the trackNames can be displayed
    tableComponent.updateContent();
    repaint();
}

void PlaylistComponent::setLoadtoDeckCallBack(LoadToDeckCallBack callback) {
    loadToDeckCallBack = callback;
}

void PlaylistComponent::textEditorTextChanged(juce::TextEditor& editor) {
    searchBarTextInput = editor.getText();
    if (!searchBarTextInput.isEmpty()) {
        applySearchFilter(searchBarTextInput);
        usingSearch = true;
    }
    else {
        usingSearch = false;
    }
    // Update and repaint the table to update the search results
    tableComponent.updateContent();
    repaint();
    DBG("PlaylistComponent::textEditorTextChanged: " + searchBarTextInput);
}

void PlaylistComponent::applySearchFilter(const juce::String& text) {
    // Clear the previous search results
    searchResultIndices.clear();

    // Loop through tracks and check if they match the search text
    for (unsigned int i = 0; i < tracks.size(); i++) {
        // Convert search text to lower case for matching
        juce::String filteredText = text.toLowerCase();
        if (tracks[i].searchableName.contains(filteredText)) {
            // If the track matches, add its index to the search results
            searchResultIndices.push_back(i);
        }
    }
    
    // for printing
    for (int index : searchResultIndices) {
        Track& track = tracks[index];
        DBG("Track: " << track.name);
    }
}

Track& PlaylistComponent::getTrackForRow(int rowNumber) {
    if (usingSearch) {
        return tracks[searchResultIndices[rowNumber]];
    }
    else {
        return tracks[rowNumber];
    }
}

void PlaylistComponent::deleteSelectedTrack(int index, bool isSearching) {
    if (isSearching) {
        if (index >= 0 && index < searchResultIndices.size()) {
            // Get the index of the track in the tracks vector
            int trackIndex = searchResultIndices[index];

            // Remove the track from both searchResultIndices and tracks
            searchResultIndices.erase(searchResultIndices.begin() + index);
            tracks.erase(tracks.begin() + trackIndex);

            // Update the searchResultIndices vector
            for (int i = index; i < searchResultIndices.size(); ++i) {
                searchResultIndices[i] = searchResultIndices[i] - 1;
            }
        }
    }
    else {
        if (index >= 0 && index < tracks.size()) {
            tracks.erase(tracks.begin() + index);
        }
    }
}

void PlaylistComponent::loadTracks() {
    // - configure the dialogue
    // The | symbol is a bitwise OR operator.
    auto fileChooserFlags =
        juce::FileBrowserComponent::canSelectFiles |
        juce::FileBrowserComponent::canSelectMultipleItems;
    // launch the file chooser
    fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
        {
            // Get an array of selected files from the file chooser
            juce::Array<juce::File> files = chooser.getResults();
            // Loop through each selected file that user have chosen and extract metadata
            for (juce::File filename : files) {
                juce::File trackFile{ filename };
                // Use the audio format reader to get the file's length and sample rate
                // we unique_ptr instead of raw pointer to prevent memory leak
                std::unique_ptr<juce::AudioFormatReader> reader(formatManagerToUse.createReaderFor(trackFile));
                Track track = track.getMetaData(std::move(reader), trackFile);
                tracks.push_back(track);
            }
            // Update the content of the table component and repaint
            tableComponent.updateContent();
            repaint();
        }
    );
}

void PlaylistComponent::importPlaylist() {
    auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
    // launch the file chooser
    fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser) {
        // Get the chosen file from the file chooser
        juce::File file = chooser.getResult();
        // Parse the chosen file as XML object
        std::unique_ptr<juce::XmlElement> xml(juce::parseXML(file));

        // Check if the XML object is valid
        if (xml == nullptr || !xml->hasTagName("Tracks")) {
            DBG("PlaylistComponent::importPlaylist null");
        }
        else {
            // Clear the current state of the tracks
            tracks.clear();
            searchResultIndices.clear();

            // Loop through each child element of the XML object
            for (auto* trackXML : xml->getChildIterator()) {
                // create new Track object from the current child element
                tracks.push_back(Track::getFromXML(trackXML));
            }            
        }

        // Update the content of the table component and repaint the playlist
        tableComponent.updateContent();
        repaint();
    });
}

void PlaylistComponent::exportPlaylist() {  
    auto fileChooserFlags = juce::FileBrowserComponent::saveMode;
    // launch the file chooser
    chooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser) {
        // Create the XML file from the chosen Directory
        chooser.getResult().create();
        // Create a File object so that we can pass into exportsTracksToXML to fill the XML file
        juce::File xmlFile = chooser.getResult();
        Track::setToXML(tracks, xmlFile);

        // Update the content of the table component and repaint the playlist
        tableComponent.updateContent();
        repaint();
    });     
}

void PlaylistComponent::addButtons() {
    // Make the components visible    
    addAndMakeVisible(loadButton);
    addAndMakeVisible(eraseButton);
    addAndMakeVisible(importButton);
    addAndMakeVisible(exportButton);
    
    // Register the components as a listeners
    loadButton.addListener(this);
    eraseButton.addListener(this);
    importButton.addListener(this);
    exportButton.addListener(this);    
}

void PlaylistComponent::addTextBoxes() {
    addAndMakeVisible(searchBar);
    searchBar.addListener(this);
    // Set the search bar text to be empty
    searchBar.setText("");
    searchBar.setColour(juce::TextEditor::backgroundColourId, juce::Colour::fromRGB(50, 50, 50));    
}

void PlaylistComponent::addTables() {
    addAndMakeVisible(tableComponent);

    loadButton.setLookAndFeel(otherLookAndFeel.get());
    eraseButton.setLookAndFeel(otherLookAndFeel.get());
    importButton.setLookAndFeel(otherLookAndFeel.get());
    exportButton.setLookAndFeel(otherLookAndFeel.get());

    tableComponent.setColour(juce::TableListBox::backgroundColourId, juce::Colour::fromRGB(40, 40, 40));
    // Add the columns to the table header
    tableComponent.getHeader().addColumn("Track Title", 1, 275);
    tableComponent.getHeader().addColumn("Format", 2, 75);
    tableComponent.getHeader().addColumn("Duration", 3, 100);
    tableComponent.getHeader().addColumn("", 4, 125);
    tableComponent.getHeader().addColumn("", 5, 125);
    tableComponent.getHeader().addColumn("", 6, 100);  
}

juce::TextButton* PlaylistComponent::createCellButton(juce::String BtnName, int rowNumber) {
    juce::TextButton* btn = new juce::TextButton{ BtnName };
    juce::String id{ std::to_string(rowNumber) };
    btn->setComponentID(id);
    btn->addListener(this);
    btn->setLookAndFeel(otherLookAndFeel.get());
    return btn;
}















