/*
  ==============================================================================

    DeckGUI.cpp
    Created: 18 Feb 2023 6:58:37pm
    Author:  SHiFT

  ==============================================================================
*/
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(
        DJAudioPlayer* _player,
        juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse
    ) : player(_player), waveformDisplay(formatManagerToUse, cacheToUse) {

    otherLookAndFeel = std::make_unique<OtherLookAndFeel>();
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(40, 0, 120));
   
    addButtons();
    addSliders();
    addLabels();
    addAndMakeVisible(waveformDisplay);
    
    // every 100ms, call the timer 
    startTimer(10);
}

DeckGUI::~DeckGUI() {
    stopTimer();
    // set it to nullptr to make sure that the sliders no longer holds a reference. hence,
    // we can prevent memory leak
    posSlider.setLookAndFeel(nullptr);
    speedSlider.setLookAndFeel(nullptr);
    playButton.setLookAndFeel(nullptr);
    stopButton.setLookAndFeel(nullptr);
    replayButton.setLookAndFeel(nullptr);
    loopButton.setLookAndFeel(nullptr);
}

void DeckGUI::paint(juce::Graphics& g) {
    g.fillAll (juce::Colour::fromRGB(40, 40, 40)); 

    g.setColour (juce::Colour::fromRGB(45, 45, 45));
    g.drawRect (getLocalBounds(), 1);

    g.setColour(juce::Colour::fromRGB(200, 200, 200));
    g.drawRect(trackNameLabel.getX(), trackNameLabel.getY(), trackDurationLabel.getRight() - trackNameLabel.getX(), trackDurationLabel.getHeight());
}

void DeckGUI::resized() {
    double rowHeight = getHeight() / 7;
    double colWidth = getWidth() / 4;
    
    volSlider.setBounds(0, 0, getWidth(), rowHeight);

    trackNameLabel.setBounds(0, rowHeight * 1.25, colWidth * 2, rowHeight);
    trackDurationLabel.setBounds(colWidth * 3.5, rowHeight * 1.25, colWidth, rowHeight);
    isLoopingLabel.setBounds(colWidth * 1.65, rowHeight * 1.25, colWidth, rowHeight);

    waveformDisplay.setBounds(0, rowHeight * 2, getWidth(), rowHeight * 2);

    playButton.setBounds(0, rowHeight * 4, colWidth, rowHeight * 0.5);
    stopButton.setBounds(colWidth, rowHeight * 4, colWidth, rowHeight * 0.5);
    replayButton.setBounds(colWidth * 2, rowHeight * 4, colWidth, rowHeight * 0.5);
    loopButton.setBounds(colWidth * 3, rowHeight * 4, colWidth, rowHeight * 0.5);

    speedSlider.setBounds(0, rowHeight * 4.75, colWidth * 2, rowHeight * 1.8);
    posSlider.setBounds(colWidth * 2, rowHeight * 4.75, colWidth * 2, rowHeight * 1.8);
}


void DeckGUI::buttonClicked(juce::Button* button) {
    if (button == &playButton) {
        player->start();
    }
    else if (button == &stopButton) {
        player->stop();
    }
    else if (button == &replayButton) {
        player->replay();
    }
    else if (button == &loopButton) {
        player->toggleLoop();
        // update the text label of looping
        if (player->getIsLooping()) {
            isLoopingLabel.setText("LOOPING", juce::dontSendNotification);
        }
        else {
            isLoopingLabel.setText("", juce::dontSendNotification);
        }
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider) {
    if (slider == &volSlider) {
        DBG("MainComponent::sliderValueChanged: volSlider" << slider->getValue());
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider) {
        DBG("MainComponent::sliderValueChanged: speedSlider" << slider->getValue());
        player->setSpeed(slider->getValue());
    }

    if (slider == &posSlider) {
        DBG("MainComponent::sliderValueChanged posSlider" << slider->getValue());
        player->setPositionRelative(slider->getValue());
    }    
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files) {
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y) {
    // print the name of the track
    for (juce::String filename : files) {
        juce::String trackName = juce::File{ filename }.getFileNameWithoutExtension();
        DBG("DeckGUI::filesDropped: " << trackName);
    }
    
    if (files.size() == 1) {
        player->loadURL(juce::URL{juce::File{files[0]}});
    }
}

void DeckGUI::timerCallback() { 
    updatePlayheadPos();
}

void DeckGUI::updatePlayheadPos() {
    double playheadPos = waveformDisplay.getPositionRelative();
    double playerPos = player->getPositionRelative();

    double durationInSeconds = player->getPosition();
    int minutes = (int)(durationInSeconds / 60);
    int seconds = (int)(durationInSeconds) % 60;
    juce::String trackDuration = juce::String::formatted("%d:%02d", minutes, seconds);

    trackDurationLabel.setText(trackDuration, juce::dontSendNotification);
    // Update the player position if the playhead has moved
    if (playheadPos != lastPlayheadPos) {
        player->setPositionRelative(playheadPos);
        lastPlayheadPos = playheadPos;
    }

    // Update the waveform display if the player has moved
    if (playerPos != lastPlayheadPos) {
        waveformDisplay.setPositionRelative(playerPos);
        lastPlayheadPos = playerPos;
    }
}

void DeckGUI::loadTrackToDeck(const juce::String& path) {
    loadURL(path);
}

void DeckGUI::loadURL(const juce::String& path) {
    juce::File file(path);
    juce::String trackName = file.getFileNameWithoutExtension();
    trackNameLabel.setText(trackName, juce::dontSendNotification);
    juce::URL url(file);
    player->loadURL(url);
    waveformDisplay.loadURL(url);
    
}

void DeckGUI::addButtons() {
    playButton.setLookAndFeel(otherLookAndFeel.get());
    stopButton.setLookAndFeel(otherLookAndFeel.get());
    replayButton.setLookAndFeel(otherLookAndFeel.get());
    loopButton.setLookAndFeel(otherLookAndFeel.get());

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(replayButton);
    addAndMakeVisible(loopButton);

    playButton.addListener(this);
    stopButton.addListener(this);
    replayButton.addListener(this);
    loopButton.addListener(this);
}

void DeckGUI::addSliders() {
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.25, 4.0);
    posSlider.setRange(0.0, 1.0);

    posSlider.setLookAndFeel(otherLookAndFeel.get());
    posSlider.setSliderStyle(juce::Slider::Rotary);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    posSlider.setTextBoxIsEditable(false);

    speedSlider.setLookAndFeel(otherLookAndFeel.get());
    speedSlider.setSliderStyle(juce::Slider::Rotary);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    speedSlider.setTextBoxIsEditable(false);

    volSlider.setTextBoxStyle(juce::Slider::TextBoxRight, true, 0, 0);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
}

void DeckGUI::addLabels() {
    trackNameLabel.setText("...", juce::dontSendNotification);
    trackNameLabel.setFont(juce::Font(16.0f));
    
    trackDurationLabel.setText("0:00", juce::dontSendNotification);
    trackDurationLabel.setFont(juce::Font(14.0f));
  
    isLoopingLabel.setText("", juce::dontSendNotification);
    isLoopingLabel.setFont(juce::Font(14.0f));

    addAndMakeVisible(isLoopingLabel);
    addAndMakeVisible(trackDurationLabel);
    addAndMakeVisible(trackNameLabel);
}









