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
#include "DelayFx.h"

class SynthVoice : public SynthesiserVoice
{
public:
    
    SynthVoice()
    {
        env.setAttack(1000);
        env.setDecay(250);
        env.setSustain(0.8);
        env.setRelease(500);
    }
    
    bool canPlaySound (SynthesiserSound *sound) override {
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }
    
    void startNote (int midiNoteNumber,
                    float velocity,
                    SynthesiserSound *sound,
                    int currerntPitchWheelPosition) override {
        env.trigger = 1;
        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        level = velocity;
    }
    
    void stopNote (float velocity, bool allowTailOff) override {
        env.trigger = 0;
        allowTailOff = true;
        if (velocity == 0) clearCurrentNote();
    }
    
    void pitchWheelMoved (int newPitchWheelValue) override {
        
    }
    
    void controllerMoved (int controllerNumber, int newControllerValue) override {
        
    }
    
    void renderNextBlock (AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override {
        
        if (outputBuffer.getNumChannels() != 2) return;
        
        for (int sample = 0; sample < numSamples; ++sample) {
            
            double sineWave = osc.sinewave(frequency);
            double sound = env.adsr(sineWave, env.trigger) * level / 2;
            
            outputBuffer.addSample(0, startSample + sample, sound);
            outputBuffer.addSample(1, startSample + sample, sound);
            
            delayFx.effect(outputBuffer, startSample + sample);
            
        }
    }
    
    void setDelaySamples(int delaySamples)
    {
        delayFx.setDelaySamples(delaySamples);
    }

private:
    double frequency;
    maxiOsc osc;
    maxiEnv env;
    float level;
    DelayFx delayFx;
};