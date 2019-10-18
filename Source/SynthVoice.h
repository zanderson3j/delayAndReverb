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
#include "fxobjects.h"

class SynthVoice : public SynthesiserVoice
{
public:
    
    SynthVoice()
    {
        delayBufferL.createCircularBuffer(88200);
        delayBufferL.flushBuffer();
        
        delayBufferR.createCircularBuffer(88200);
        delayBufferR.flushBuffer();
        
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
            
            float ynL = delayBufferL.readBuffer(24000);
            float y1L = delayBufferL.readBuffer(12000)*0.5;
            float y2L = delayBufferL.readBuffer(8000)*0.5;
            
            float ynR = delayBufferR.readBuffer(12000);
            float y1R = delayBufferR.readBuffer(8000)*0.5;
            float y2R = delayBufferR.readBuffer(4000)*0.5;
            
            
//                        float y1L = 0;
//                        float y2L = 0;
//                        float y1R = 0;
//                        float y2R = 0;
            
            float dnL = outputBuffer.getSample(0, startSample + sample)*0.5 + (0.8*ynL);
            float dnR = outputBuffer.getSample(1, startSample + sample)*0.5  + (0.4*ynR);
            
            delayBufferL.writeBuffer(dnR);
            delayBufferR.writeBuffer(dnL);
            
            outputBuffer.addSample(0,
                                   startSample + sample,
                                   (outputBuffer.getSample(0, startSample + sample)*0.5
                                   + (ynL + y1L + y2L)*0.5) * 0.01);
            outputBuffer.addSample(1,
                                   startSample + sample,
                                   (outputBuffer.getSample(1, startSample + sample)*0.5
                                   + (ynR + y1R + y2R)*0.5) * 0.01);
            
        }
    }

private:
    double frequency;
    maxiOsc osc;
    maxiEnv env;
    float level;
    CircularBuffer<float> delayBufferL;
    CircularBuffer<float> delayBufferR;
};