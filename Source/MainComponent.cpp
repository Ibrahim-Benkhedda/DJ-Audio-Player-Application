#include "MainComponent.h"

MainComponent::MainComponent() {
    // Make sure you set the size of the component after
    // you add any child components.
    setSize(800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(2, 2);
    }
    
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(deck1Label);
    addAndMakeVisible(deck2Label);
    addAndMakeVisible(playlistComponent);

    deck1Label.setColour(juce::Label::textColourId, juce::Colours::white);
    deck1Label.setFont(18.0f);
    deck2Label.setColour(juce::Label::textColourId, juce::Colours::white);
    deck2Label.setFont(18.0f);
    deck1Label.setText("DECK 1", juce::dontSendNotification);
    deck2Label.setText("DECK 2", juce::dontSendNotification);

    formatManager.registerBasicFormats();

    /** Set the callback function in PlaylistComponent to handle loading tracks into the deck */
    playlistComponent.setLoadtoDeckCallBack([this](const juce::String& path, int deckNum) {
        // Call the method handleLoadToDeck when the callback is triggered
        handleLoadToDeck(path, deckNum);
    });
}

MainComponent::~MainComponent() {
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    // Prepare DJAudioPlayer objects for playback
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    // Prepare MixerAudioSource for playback and add input sources
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);    
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);   
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    // Passes audio blocks from MixerAudioSource to AudioAppComponent buffer for playback
    mixerSource.getNextAudioBlock(bufferToFill);

}

void MainComponent::releaseResources() {
    // Releases resources of DJAudioPlayer and MixerAudioSource objects
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

void MainComponent::paint(juce::Graphics& g) {
   
    g.fillAll(juce::Colour::fromRGB(40, 0, 120));

    g.setColour(juce::Colour::fromRGB(60, 60, 60));
    g.drawRect(0, 0, getWidth() / 2, 25);
    g.drawRect(0, 0, getWidth(), 25);
}

void MainComponent::resized() {  
    int deckWidth = getWidth() / 2;
    int deckHeight = getHeight() / 2;
    int startDeckYPos = 25;

    deck1Label.setBounds((deckWidth / 2) - (getWidth() / 20), 0, deckWidth, 20);
    deck2Label.setBounds(deckWidth + (deckWidth / 2) - (getWidth() / 20), 0, deckWidth, 20);
    deckGUI1.setBounds(0, startDeckYPos, deckWidth, deckHeight);
    deckGUI2.setBounds(deckWidth, startDeckYPos, deckWidth, deckHeight);

    playlistComponent.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);
}

void MainComponent::loadTrackToDeckGUI(const juce::String& path, int deckNum) {
    // Determine which DeckGUI to load the track into
    if (deckNum == 1) {
        deckGUI1.loadTrackToDeck(path);
    }
    else if (deckNum == 2) {
        deckGUI2.loadTrackToDeck(path);
    }
}

void MainComponent::handleLoadToDeck(const juce::String& path, int deckNum) {
    // Call the function to load the track to the corresponding DeckGUI
    loadTrackToDeckGUI(path, deckNum);
}





