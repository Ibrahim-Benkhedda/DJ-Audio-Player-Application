/*
  ==============================================================================

    DeckGUI represents the GUI for one deck of the Application.
    - create buttons, sliders, and labels, and it interacts with a DJAudioPlayer object
    to play audio tracks. It defines methods to handle button and slider events.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "OtherLookAndFeel.h"

//==============================================================================
/*
*/
class DeckGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::FileDragAndDropTarget,
    public juce::Timer
{
    public:
        DeckGUI(DJAudioPlayer* player,
                juce::AudioFormatManager& formatManagerToUse,
                juce::AudioThumbnailCache& cacheToUse);
        ~DeckGUI() override;

        /** Override of the paint method inherited from Component */
        void paint(juce::Graphics&) override;
        /** Override of the resized method inherited from Component */
        void resized() override;
     
        // implement Button and slider Listener 
        void buttonClicked(juce::Button*) override;
        /**  listener */
        void sliderValueChanged(juce::Slider* slider) override;
        // Drag and drop callbacks
        bool isInterestedInFileDrag(const juce::StringArray &files) override;
        void filesDropped(const juce::StringArray &files, int x, int y) override;

        /**  Timer callback  */
        void timerCallback() override;

        /** load track to the get from the parsed track path by calling loadURL */
        void loadTrackToDeck(const juce::String& path);

        /** loads the track URL to the waveform and audio player */
        void loadURL(const juce::String& path);

    private:
        // Button components
        juce::TextButton playButton{ "PLAY" };
        juce::TextButton stopButton{ "STOP" };
        juce::TextButton replayButton{ "REPLAY" };
        juce::TextButton loopButton{ "LOOP" };

        // Slider components
        juce::Slider volSlider;
        juce::Slider speedSlider;
        juce::Slider posSlider;

        // Label components
        juce::Label trackNameLabel;
        juce::Label trackDurationLabel;
        juce::Label isLoopingLabel;

        // Private methods to add child components
        void addButtons();
        void addSliders();
        void addLabels();
        // update the playhead position 
        void updatePlayheadPos();

        // File chooser dialog for opening audio files
        juce::FileChooser fChooser{ "Select a file..." };

        // Pointer to the DJAudioPlayer object that this GUI controls
        DJAudioPlayer* player;

        // WaveformDisplay object
        WaveformDisplay waveformDisplay;

        // last playhead position 
        double lastPlayheadPos{ 0.0 };

        // Custom LookAndFeel object for customizing the component's appearance
        std::unique_ptr<OtherLookAndFeel> otherLookAndFeel;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
