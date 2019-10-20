/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAndReverbAudioProcessorEditor::DelayAndReverbAudioProcessorEditor (DelayAndReverbAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    delayTime.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    delayTime.setRange(0.0, 2000.0);
    delayTime.setValue(0.0);
    delayTime.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    delayTime.addListener(this);
    addAndMakeVisible(&delayTime);
    
    sliderTree = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.tree,
                                                                    "delayTime",
                                                                    delayTime);
}

DelayAndReverbAudioProcessorEditor::~DelayAndReverbAudioProcessorEditor()
{
}

//==============================================================================
void DelayAndReverbAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::blueviolet);

    g.setColour (Colours::antiquewhite);
    g.setFont (15.0f);
    g.drawFittedText ("DROP-shot", getLocalBounds(), Justification::centred, 1);
}

void DelayAndReverbAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    delayTime.setBounds(100, 140, 200, 50);
}

void DelayAndReverbAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &delayTime)
    {
        processor.setDelayInMilis(delayTime.getValue());
    }
}
