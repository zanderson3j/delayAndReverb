/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DelayAndReverbAudioProcessorEditor  : public AudioProcessorEditor,
                                            public Slider::Listener
{
public:
    DelayAndReverbAudioProcessorEditor (DelayAndReverbAudioProcessor&);
    ~DelayAndReverbAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged (Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayAndReverbAudioProcessor& processor;
    Slider delayTime;
    Slider reverbTime;
    unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sliderTreeDelay;
    unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sliderTreeReverb;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAndReverbAudioProcessorEditor)
};
