/*
  ==============================================================================

    Track is a class that represents an audio track. it contains several member variables
    that store and get metadata about the track. The Track class also contains static methods
    for exporting and importing track to/from an XML file.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Track
{
    public:
        /** tells the compiler to generate default constructor for the Track class */
        Track() = default;
        /** Constructor. Initializes the member variables with the provided values.*/
        Track(juce::String name, juce::String extension,
                juce::String artist, juce::String path,
                juce::String duration, juce::String searchableName );

        // meta data of the audio track
        juce::String name;
        juce::String extension;
        juce::String artist;
        juce::String path;
        juce::String duration;
        // searchable name is the trackname but lowercase, we use it for the searching
        juce::String searchableName;

        /** Returns the duration of the audio track in seconds */
        double Track::getDuration(juce::AudioFormatReader* reader);
        /** Returns the formatted string representation of the duration of an audio track (minutes:seconds) */
        juce::String Track::getFormattedDuration(double durationInSeconds);      
        /** Returns a the meta data of the Audio file in form of a Track object.
            Takes a unique pointer to an AudioFormatReader and a File object. */
        Track Track::getMetaData(std::unique_ptr<juce::AudioFormatReader>&& reader, juce::File trackFile);
        /** static function that exports/saves the track into an XML File */
        static void setToXML(std::vector<Track>& tracks, juce::File file);
        /** static function that returns Track object from parsed XML */
        static Track getFromXML(juce::XmlElement* parsedXML);

};