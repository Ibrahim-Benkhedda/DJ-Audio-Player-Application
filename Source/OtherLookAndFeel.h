/*
  ==============================================================================

    OtherLookAndFeel class represents a custom LookAndFeel class to provide a new visual style
    for Slider and Buttons. This class contains methods that define custom drawing routines
    for specific components, and override the default LookAndFeel provided by JUCE.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class OtherLookAndFeel : public juce::LookAndFeel_V4
{
    public: 
        /** Constructor */ 
        OtherLookAndFeel();

        /** Function to draw a rotary slider, Overrides the drawRotarySlider function */
        void drawRotarySlider(juce::Graphics& g,
                                 int x, int y, int width, int height, float sliderPos,
                                 const float rotaryStartAngle, const float rotaryEndAngle,
                                 juce::Slider& slider);
        /** Function to draw a button background, Overrides the drawButtonBackground function */
        void drawButtonBackground(juce::Graphics& g,
                                    juce::Button& button,
                                    const juce::Colour& backgroundColour,
                                    bool shouldDrawButtonAsHighlighted,
                                    bool shouldDrawButtonAsDown) override;

        /** Function to draw button text, Overrides the drawButtonText function */
        void drawButtonText(juce::Graphics& g,
                            juce::TextButton& button,
                            bool shouldDrawButtonAsHighlighted,
                            bool shouldDrawButtonAsDown) override;
};