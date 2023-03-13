/*
  ==============================================================================

    WaveformDisplay is a custom component that displays the waveform of an audio file
    and allows the user to interact with it by clicking and dragging mouse to change
    the playhead position.
    It uses the JUCE AudioThumbnail class to generate and display the waveform.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class WaveformDisplay : public juce::Component,
    public juce::ChangeListener
{
    public:
        /** Constructor */
        WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                        juce::AudioThumbnailCache& cacheToUse);
        /** Deconstructor */
        ~WaveformDisplay() override;
        /** Virtual function of Component, gets called when a region of a component needs redrawing */
        void paint (juce::Graphics&) override;
        /** Virtual function of Component, gets called when this component's size has been changed */
        void resized() override;

        /** pure virtual function of the ChangeListener Class, receive the callback */
        void changeListenerCallback(juce::ChangeBroadcaster *source) override;
        
        /** Loads an audio file from the specified URL and creates an AudioThumbnail
            object to represent the waveform */
        void loadURL(juce::URL audioURL);
        /** set the relative position of the playhead */
        void setPositionRelative(double pos);
        /** get the relative position of the playhead */
        double getPositionRelative();
        
    private:
        // provides functions to read an audio file then to plot it(loadFrom and drawChannel/drawChannels)
        juce::AudioThumbnail audioThumb;
        bool fileLoaded;
        double position;

        /** This method gets called when the user drags the mouse over the waveform */
        void mouseDown(const juce::MouseEvent& event) override;
        /** This method gets called when the user drags the mouse over the waveform */
        void mouseDrag(const juce::MouseEvent& event) override;
        /** This method gets called when the user drags the mouse over the waveform */
        void mouseUp(const juce::MouseEvent& event) override;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
