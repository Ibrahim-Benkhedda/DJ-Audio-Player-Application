#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                 juce::AudioThumbnailCache& cacheToUse)
    : audioThumb(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0) {
    audioThumb.addChangeListener(this);

    // Add the mouse listener
    addMouseListener(this, true);
    setInterceptsMouseClicks(true, true);
}

WaveformDisplay::~WaveformDisplay() {

}

void WaveformDisplay::paint (juce::Graphics& g) {
    g.fillAll (juce::Colour::fromRGB(50, 50, 50));   // clear the background

    g.setColour (juce::Colour::fromRGB(45, 45, 45));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colour::fromRGB(249, 68, 72));
    if (fileLoaded) {
        audioThumb.drawChannel(g,
            getLocalBounds(),
            0,
            audioThumb.getTotalLength(),
            0,
            1.0f
        );

        // draws the playhead
        g.setColour(juce::Colour::fromRGB(229, 52, 11));
        g.fillRect(position * getWidth(), 0, getWidth() / 80, getHeight());
        g.setColour(juce::Colour::fromRGB(229, 52, 11));
        g.drawRect(position * getWidth(), 0, getWidth() / 75, getHeight());
        g.setColour(juce::Colour::fromRGB(168, 36, 55));
        g.fillRect(position * getWidth() - 3, 0, getWidth() / 100 + 7, 5);
        g.drawRect(position * getWidth() - 3, 0, getWidth() / 100 + 7, 5);
        g.setColour(juce::Colour::fromRGB(229, 52, 11));
        g.fillRect(position * getWidth() - 3, getHeight() - 5, getWidth() / 100 + 7, getHeight());
        g.drawRect(position * getWidth() - 3, getHeight() - 5, getWidth() / 100 + 7, getHeight());


        // Draw the waveform past the playhead position
        g.setColour(juce::Colour::fromRGBA(0, 0, 0, 75));     
        g.fillRect(0, 0, position * getWidth(), getHeight());

    }
    else {
        g.setColour(juce::Colours::white);
        g.setFont(20.0f);
        g.drawText("Track not loaded", getLocalBounds(),
            juce::Justification::centred, true);   // draw some placeholder text
    }  
}

void WaveformDisplay::resized() {

}

void WaveformDisplay::loadURL(juce::URL audioURL) {
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
    if (fileLoaded) {
        repaint();
    }
    else {
        repaint();
    }
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source) {
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos) {
    if (pos != position) {
        position = pos;
        repaint();
    }   
}

double WaveformDisplay::getPositionRelative() {
    return position;
}

void WaveformDisplay::mouseDown(const juce::MouseEvent& event) {
    if (fileLoaded) {
        // this updates the position to where the user clicked on the waveform
        double playheadPos = (double)event.x / getWidth();
        setPositionRelative(playheadPos);
    }
}

void WaveformDisplay::mouseDrag(const juce::MouseEvent& event) {
    if (fileLoaded) {
        // this updates the position to where the user clicked on the waveform
        double playheadPos = (double)event.x / getWidth();
        setPositionRelative(playheadPos);
    }
}

void WaveformDisplay::mouseUp(const juce::MouseEvent& event) {
    if (fileLoaded) {
        // this updates the position to where the user clicked on the waveform
        double playheadPos = (double)event.x / getWidth();
        setPositionRelative(playheadPos);
    }
}


