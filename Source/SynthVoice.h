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

class SynthVoice : public SynthesiserVoice
{
public:
    
    bool canPlaySound (SynthesiserSound *sound) override {
        return true;
    }
    
    void startNote (int midiNoteNumber,
                    float velocity,
                    SynthesiserSound *sound,
                    int currerntPitchWheelPosition) override {
        
    }
    
    void stopNote (float velocity, bool allowTailOff) override {
        
    }
    
    void pitchWheelMoved (int newPitchWheelValue) override {
        
    }
    
    void controllerMoved (int controllerNumber, int newControllerValue) override {
        
    }
    
    void renderNextBlock (AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override {
        
    }

private:
};