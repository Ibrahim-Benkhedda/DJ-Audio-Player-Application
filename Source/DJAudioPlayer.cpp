/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 18 Feb 2023 12:22:43pm
    Author:  Ibrahim

  ==============================================================================
*/

#include "DJAudioPlayer.h"



DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager) {
  
    
}
DJAudioPlayer::~DJAudioPlayer() {

}

//==============================================================================
// BIRTH
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
// LIFE, "like the draw function?"
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    resampleSource.getNextAudioBlock(bufferToFill);
    checkLooping(isLooping);

}
// DEATH
void DJAudioPlayer::releaseResources() {
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

//==============================================================================
void DJAudioPlayer::loadURL(juce::URL audioURL) {
    // creates an audio reader object that reads audio data from audio file.
    // the input stream represents the audio file.
    // formatManager determines the audio format of the file and creates the object
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));

    // checks if successfuly created the audio reader
    if (reader != nullptr)
    {
        // create an object that represents the audio data that will be played back.
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        // sets the AudioFormatReaderSource object newSource as the audio source
        // for the transportSource object, which is a juce::TransportSource object.
        // The 0 parameter specifies the start sample of the audio source,
        // and nullptr specifies the time stretch algorithm to use
        // The reader->sampleRate parameter specifies the sample rate of the audio data,
        // which is obtained from the AudioFormatReader object that was created earlier.
        transportSource.setSource(newSource.get(),
            0, nullptr, reader->sampleRate);
        // resets the reader Source
        readerSource.reset(newSource.release());
    }
    else
    {
        DBG("Something went wrong loading the file ");
    }
}
void DJAudioPlayer::setGain(double gain) {
    if (gain < 0 || gain > 1.0) {
        DBG(" DJAudioPlayer::setGain gain should be [0, 1]");
    }
    else {
        transportSource.setGain(gain);
    }

}

void DJAudioPlayer::setSpeed(double ratio) {
    if (ratio < 0 || ratio > 100.0) {
        DBG(" DJAudioPlayer::setSpeed ratio should be [0, 100]");
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(double posInSecs) {
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos) {
    if (pos < 0 || pos > 1) {
        DBG(" DJAudioPlayer::setPositionRelative pos should be [0, 1]");
    }
    else {
        // calculates the absolute position in secs by
        // multiplying the total duration of the audio file (getLengthinSeconds) by
        // pos (position of the slider)
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

double DJAudioPlayer::getPositionRelative() {
    // if streams length is 0, prevent the it from dividing by 0, else 
    // return the position in seconds divided by the stream's length in seconds
    if (transportSource.getLengthInSeconds() == 0) {
        return 0;
    }
    else {
        return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
    }
}

void DJAudioPlayer::start() {
    transportSource.start();
}

void DJAudioPlayer::stop() {
    transportSource.stop();
}

void DJAudioPlayer::replay() {
    transportSource.setPosition(0);
    transportSource.start();
}

void DJAudioPlayer::toggleLoop() {
    isLooping = !isLooping;
    checkLooping(isLooping);
}

void DJAudioPlayer::checkLooping(bool isLooping) {
    // Check if the current playback position has exceeded the end of the song
    if (isLooping && transportSource.getCurrentPosition() >= transportSource.getLengthInSeconds()) {
        // Set the playback position back to the beginning of the song
        transportSource.setPosition(0);
        transportSource.start();
    }
}

bool DJAudioPlayer::getIsLooping() {
    return isLooping;
}

double DJAudioPlayer::getPosition() {
    return transportSource.getCurrentPosition();
}

bool DJAudioPlayer::isLoopingEnabled() {
    return isLooping;
}
