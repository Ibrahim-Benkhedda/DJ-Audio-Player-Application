/*
  ==============================================================================

    PlaylistComponent is class that represents the implementation of the playlist GUI. It displays
    a library of audio tracks. allows the user load the tracks into a DECKGUI. it also allows
    the user to delete, tracks from the current playlist.

    The class uses a vector of Track objects to store the tracks and provides implementations of several
    virtual functions required by TableListBoxModel interface to display the track metadata in a Playlist
    table.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Track.h"
#include "OtherLookAndFeel.h"

/*
    - TableListBoxModel is sort of a helper class/object to work with ListBox to help it draw it cells 
    - it has to know the data of the cells 
*/
class PlaylistComponent : public juce::Component,
    public juce::TableListBoxModel,
    public juce::Button::Listener,
    public juce::FileDragAndDropTarget,
    public juce::TextEditor::Listener,
    public juce::XmlElement
{
    public:
        /** Constructor for the PlaylistComponent that takes an AudioFormatManager reference */
        PlaylistComponent(juce::AudioFormatManager& formatManagerToUse);
        /** Deconstructor */
        ~PlaylistComponent() override;
        /** Override of the paint method inherited from Component */
        void paint (juce::Graphics&) override;
        /** Override of the resized method inherited from Component */
        void resized() override;
        /** returns the number of items in the list */
        int getNumRows() override;

        /** draw the background behind one of the rows in the table */
        void paintRowBackground(juce::Graphics&, int rowNumber, int width,
                                 int height, bool rowIsSelected) override;
        /** draws one of the cells */
        void paintCell(juce::Graphics&, int rowNumber, int columnId,
                        int width, int height, bool rowIsSelected) override;
        /** create or update a custom component to go in a cell */
        juce::Component* refreshComponentForCell(int rowNumber, int columnId,
                                                 bool isRowSelected,
                                                 juce::Component* existingComponentToUpdate) override;

        /** Called when the button's state changes */
        void buttonClicked(juce::Button* button) override;

        /** check whether this target is interested in the set of files being offered.
            this will be called repeatedly when the user is dragging the mouse around
            Playlist */
        bool isInterestedInFileDrag(const juce::StringArray& files) override;
        /** Callback to indicate that the user has dropped the files onto this component.
            When the user drops the files, this get called */
        void filesDropped(const juce::StringArray& files, int x, int y) override; 

        /** Called when the user changes the text*/
        void textEditorTextChanged(juce::TextEditor& editor) override;

        /** defines a type alias for a function that takes a juce::String and an int as arguments.
            - the string represents the Path of the file.
            - the int represents the number of which Deck to load into */
        using LoadToDeckCallBack = std::function<void(const juce::String&, int)>;
        /** Sets a callback function to be called when a track want a track want to be loaded */
        void setLoadtoDeckCallBack(LoadToDeckCallBack callback);  

    private:
        juce::TableListBox tableComponent;

        /** load button */
        juce::TextButton loadButton{ "LOAD TRACKS" };
        /** delete all tracks button */
        juce::TextButton eraseButton{ "DELETE TRACKS" };
        /** import Playlist button */
        juce::TextButton importButton{ "IMPORT PLAYLIST" };
        /** export the current Playlist button*/
        juce::TextButton exportButton{ "EXPORT PLAYLIST" };
   
        /** Text Editor that is responsible for the search bar */
        juce::TextEditor searchBar{ "SEARCH" };

        juce::FileChooser fChooser{ "Select a file..." };
        /** FileChooser to export xml type file only */
        juce::FileChooser exportChooser{ "Export Playlist", {}, "*.xml"};
        /** File chooser that saves the file*/
        juce::FileChooser chooser{
            "Save file", juce::File::getSpecialLocation(juce::File::userMusicDirectory),
            "*.xml", true };

        /** stores the tracks as a vector of Track objects */
        std::vector<Track> tracks;
        int currentIndex;

        LoadToDeckCallBack loadToDeckCallBack;
        juce::AudioFormatManager& formatManagerToUse;

        /** User input from the search bar text editor component */
        juce::String searchBarTextInput;

        std::vector<int> searchResultIndices;
        /** Apply the filtering of the search based on the text editor value */
        void applySearchFilter(const juce::String& filterText);
        /** boolean value to check if the user is using the search bar or not */
        bool usingSearch{ false };
        Track& PlaylistComponent::getTrackForRow(int rowNumber);

        /**
            deletes the selected track from the playlist table.
            pass id as first argument, and boolean value if the user is searching
            or not as the second argument
        */
        void deleteSelectedTrack(int id, bool isSearching);  
        /** load Tracks from File Chooser */
        void loadTracks();
        /** export the tracks into an XML file */
        void exportPlaylist();
        /** import tracks from an XML file */
        void importPlaylist();

        std::unique_ptr<OtherLookAndFeel> otherLookAndFeel;

        /** adds, register and make visible button objects */
        void addButtons();
        /** adds, register and make visible TextBox objects */
        void addTextBoxes();
        /** adds, register and make visible Table objects */
        void addTables();

        juce::TextButton* createCellButton(juce::String BtnName, int rowNumber);
      
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
