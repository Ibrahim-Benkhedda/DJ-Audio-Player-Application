#include "Track.h"

Track::Track(juce::String name, juce::String extension,
    juce::String artist, juce::String path,
    juce::String duration, juce::String searchableName
    ) : name(name), extension(extension), artist(artist),
    path(path), duration(duration), searchableName(searchableName) {
  
}

double Track::getDuration(juce::AudioFormatReader* reader) {
    // Get the length of the audio track in samples
    int lengthInSamples = reader->lengthInSamples;
    // Get the sample rate of the audio track
    int sampleRate = reader->sampleRate;
    // Calculate and convert to double the duration of the audio track in seconds and return it.
    return (double)lengthInSamples / (double)sampleRate;
}

juce::String Track::getFormattedDuration(double durationInSeconds) {
    // Calculate the number of minutes in the duration
    int minutes = (int)(durationInSeconds / 60);
    // Calculate the number of minutes in the seconds
    int seconds = (int)(durationInSeconds) % 60;
    // Return formatted duration [mm:ss]. (%02d) represents two decimals in seconds
    return juce::String::formatted("%d:%02d", minutes, seconds);
}

Track Track::getMetaData(std::unique_ptr<juce::AudioFormatReader>&& reader, juce::File trackFile) {
    // create new Track object 
    Track track;
    track.name = trackFile.getFileNameWithoutExtension();
    track.searchableName = track.name.toLowerCase();
    track.extension = trackFile.getFileExtension();
    track.artist = "";
    track.path = trackFile.getFullPathName();
    track.duration;
    // If the AudioFormatReader pointer is null, set the duration to "??:??" else compute the duration
    if (reader == nullptr) {
        track.duration = "??:??";
    }
    else {
        // Format the track duration as a string
        double trackDurationInSeconds = getDuration(reader.get());
        track.duration = getFormattedDuration(trackDurationInSeconds);
    }

    // Return the Track object
    return track;
}

void Track::setToXML(std::vector<Track>& tracks, juce::File file) {
    // create XML object that represents the root element of the XML File.
    juce::XmlElement xml("Tracks");

    // loop through each track in tracks and create a child element in the XML object
    for (const Track& track : tracks) {
        // create a unique pointer to a new XmlElement with the name "Track"
        std::unique_ptr<juce::XmlElement> trackXML(new juce::XmlElement("Track"));
        // set the attributes of the new Track element
        trackXML->setAttribute("name", track.name);
        trackXML->setAttribute("extension", track.extension);
        trackXML->setAttribute("artist", track.artist);
        trackXML->setAttribute("searchableName", track.searchableName);
        trackXML->setAttribute("duration", track.duration);
        trackXML->setAttribute("path", track.path);
        // Add the trackXML as child element and release the ownership of the pointer
        xml.addChildElement(trackXML.release());
    }
    // Save the XML object to the file
    xml.writeToFile(file, "");
}

Track Track::getFromXML(juce::XmlElement* parsedXML) {
    Track track;

    // get the attributes of the parsed track element
    track.name = parsedXML->getStringAttribute("name");
    track.extension = parsedXML->getStringAttribute("extension");
    track.artist = parsedXML->getStringAttribute("artist");
    track.searchableName = parsedXML->getStringAttribute("searchableName");
    track.duration = parsedXML->getStringAttribute("duration");
    track.path = parsedXML->getStringAttribute("path");

    // Return the Track object
    return track;
}