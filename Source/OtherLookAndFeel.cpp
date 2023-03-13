#include "OtherLookAndFeel.h"

OtherLookAndFeel::OtherLookAndFeel() {
    setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(40, 0, 120));
}

void OtherLookAndFeel::drawRotarySlider(juce::Graphics& g,
                                        int x, int y, int width, int height, float sliderPos,
                                        const float rotaryStartAngle,  const float rotaryEndAngle,
                                        juce::Slider& slider) {
    // taken from LookAndFeel demo project of JUCE tutorials.
    // Calculate the radius of the rotary slider
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    // Calculate the center x and y coordinates of the rotary slider
    auto centerX = (float)x + (float)width * 0.5f;
    auto centerY = (float)y + (float)height * 0.5f;
    // Calculate the x and y coordinates of the top left corner of the bounding box
    auto rx = centerX - radius;
    auto ry = centerY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    g.setColour(juce::Colour::fromRGB(40, 0, 120));
    
    g.fillEllipse(rx, ry, rw, rw);

    g.setColour(juce::Colours::white);
    g.drawEllipse(rx, ry, rw, rw, 2.0f);

    // Create a path for the pointer of the rotary slider
    juce::Path p;
    auto pointerLength = radius * 0.66f;
    auto pointerThickness = 5.0f;
    // Add a rectangle shape to the path that represents the pointer
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    // Applies a rotation transformation to the path based on the angle of the rotary slider
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centerX, centerY));


    g.setColour(juce::Colours::red);
    g.fillPath(p);
}

void OtherLookAndFeel::drawButtonBackground(juce::Graphics& g,
                                            juce::Button& button,
                                            const juce::Colour& backgroundColour,
                                            bool shouldDrawButtonAsHighlighted,
                                            bool shouldDrawButtonAsDown) {
    
    // checks if mouse is over button
    if (button.isMouseOver()) 
    {
        g.setColour(juce::Colour::fromRGB(40, 0, 120));
        g.fillAll();

    }
    else {
        g.setColour(juce::Colour::fromRGB(45, 45, 45));
        g.fillAll();
    }

    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(button.getLocalBounds().toFloat(), 0, 1); // draws a white border with 5 pixels corner size and 2 pixels thickness
}

void OtherLookAndFeel::drawButtonText(juce::Graphics& g,
                                        juce::TextButton& button,
                                        bool shouldDrawButtonAsHighlighted,
                                        bool shouldDrawButtonAsDown) {
    // sets the text colour to white
    g.setColour(juce::Colours::white); 
    g.drawText(button.getButtonText(),
        button.getLocalBounds(),
        juce::Justification::centred,
        true
    );
}