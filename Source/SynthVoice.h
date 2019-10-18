/*
  ==============================================================================

    SynthVoice.h
    Created: 17 Oct 2019 10:01:11pm
    Author:  Zachary Anderson

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "maximilian.h"

class SynthVoice : public SynthesiserVoice
{
public:
    
    bool canPlaySound (SynthesiserSound *sound) override {
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }
    
    void startNote (int midiNoteNumber,
                    float velocity,
                    SynthesiserSound *sound,
                    int currerntPitchWheelPosition) override {
        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        level = velocity;
    }
    
    void stopNote (float velocity, bool allowTailOff) override {
        level = 0;
    }
    
    void pitchWheelMoved (int newPitchWheelValue) override {
        
    }
    
    void controllerMoved (int controllerNumber, int newControllerValue) override {
        
    }
    
    void renderNextBlock (AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override {
        
        for (int sample = 0; sample < numSamples; ++sample) {
            
            double sineWave = osc.sinewave(frequency) * level;
            
            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
                outputBuffer.addSample(channel, startSample + sample, sineWave);
            }
        }
    }

private:
    double frequency;
    maxiOsc osc;
    float level;
};