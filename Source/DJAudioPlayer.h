/*
  ==============================================================================
    
    DJAudioPlayer represents the interface for controling and loading and playing audio files 

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource
{
    public:
        DJAudioPlayer(juce::AudioFormatManager& _formatManager);
        ~DJAudioPlayer();
    
        /** Overrides AudioSource's prepareToPlay method.
        initializes the audio transport source's buffer size and sample rate */
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        /** Overrides AudioSource's getNextAudioBlock method.
            copies the next block of audio samples into the parsed buffer*/
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
        /* Overrides AudioSource's releaseResources method. stops and releases the audio
           transport source. */
        void releaseResources() override;
        
        /** loads audio file from the parsed audio URL and initializes the audio transport source */
        void loadURL(juce::URL audioURL);
        /** set playback gain */
        void setGain(double gain);
        /** set the playback speed */
        void setSpeed(double ratio);
        /** set the position of the audio playback in seconds */
        void setPosition(double posInSecs);
        /** set the position of the audio playback relative to its current position */
        void setPositionRelative(double pos);
        /** get the current position of the audio playback in seconds */
        double getPosition();
        /** checks if looping is enabled */
        bool isLoopingEnabled();
        /** start the audio playback */
        void start();
        /** stop the audio playback */
        void stop();
        /** replay the audio playback from 0*/
        void replay();
        /** toggle looping on/off */
        void toggleLoop();
        /** checks the state of looping */
        bool getIsLooping();
        /** get the relative position of the playhead */
        double getPositionRelative();

    private:
        // Three components needed for audio file playback //
        // pass into DJ Audio player by reference
        juce::AudioFormatManager& formatManager;
        // smart pointer for better memory managment and follows pattern of JUCE
        std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
        // allows us to have control over the audio playbacks (stop, start, go back etc..)
        juce::AudioTransportSource transportSource;
        juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

        /** Boolean whether looping is enabled */
        bool isLooping{ false };
        /** checks if looping is possible */
        void checkLooping(bool isLooping);
    
};

