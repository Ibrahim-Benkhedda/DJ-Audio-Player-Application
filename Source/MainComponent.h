/*
  ==============================================================================
	MainComponent class is the central class of the whole Application. it provides
	audio playback functionality by using two (player1, player2) instances of
	the DJAudioPlayer class. It also contains two (deckGUI1, deckGUI2) instances
	of the DeckGUI class

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

class MainComponent : public juce::AudioAppComponent
{
	public:
		/** Constructor */
		MainComponent();
		~MainComponent() override;

		/**  Override of the prepareToPlay method inherited from AudioAppComponent */
		void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
		/**  Override of the getNextAudioBlock method inherited from AudioAppComponent */
		void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
		/**  Override of the releaseResources method inherited from AudioAppComponent */
		void releaseResources() override;

		/** Override of the paint method inherited from Component */
		void paint(juce::Graphics& g) override;
		/** Override of the resized method inherited from Component */
		void resized() override;

		/** Handle loading a track onto a deck by passing the track's path and the deck number to the loadToDeck */
		void handleLoadToDeck(const juce::String& path, int deckNum);
   
	private:
		juce::AudioFormatManager formatManager;
		// stores 100 cache waveforms
		juce::AudioThumbnailCache thumbCache{100};

		DJAudioPlayer player1{formatManager};
		DJAudioPlayer player2{formatManager};
		DeckGUI deckGUI1{ &player1, formatManager, thumbCache };
		DeckGUI deckGUI2{ &player2, formatManager, thumbCache };

		juce::MixerAudioSource mixerSource;

		PlaylistComponent playlistComponent {formatManager};

		/** Load track onto a deck. first agument is juce string of the file path. second is the deck number */
		void loadTrackToDeckGUI(const juce::String& path, int deckNum);

		juce::Label deck1Label { "DECK 1" };
		juce::Label deck2Label { "DECK 2" };
 
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent);
};