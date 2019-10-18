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
    }
    
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
        
        if (outputBuffer.getNumChannels() != 2) return;
        
        for (int sample = 0; sample < numSamples; ++sample) {
            
            double sineWave = osc.sinewave(frequency) * level;
            
            outputBuffer.addSample(0, startSample + sample, sineWave);
            outputBuffer.addSample(1, startSample + sample, sineWave);
            
            float ynL = delayBufferL.readBuffer(24000);
            float y1L = delayBufferL.readBuffer(12000)*0.5;
            float y2L = delayBufferL.readBuffer(8000)*0.5;
            float y3L = delayBufferL.readBuffer(4000)*0.5;
            float y4L = delayBufferL.readBuffer(2000)*0.5;
            
            float ynR = delayBufferR.readBuffer(12000);
            float y1R = delayBufferR.readBuffer(8000)*0.5;
            float y2R = delayBufferR.readBuffer(4000)*0.5;
            float y3R = delayBufferR.readBuffer(2000)*0.5;
            float y4R = delayBufferR.readBuffer(1000)*0.5;
            
            float dnL = outputBuffer.getSample(0, startSample + sample)*0.5 + (0.2*ynL);
            float dnR = outputBuffer.getSample(1, startSample + sample)*0.5  + (0.5*ynR);
            
            delayBufferL.writeBuffer(dnR);
            delayBufferR.writeBuffer(dnL);
            
            outputBuffer.addSample(0,
                                   startSample + sample,
                                   outputBuffer.getSample(0, startSample + sample)*0.5
                                   + (ynL + y1L + y2L + y3L + y4L)*0.5);
            outputBuffer.addSample(1,
                                   startSample + sample,
                                   outputBuffer.getSample(1, startSample + sample)*0.5
                                   + (ynR + y1R + y2R + y3R + y4R)*0.5);
            
        }
    }

private:
    double frequency;
    maxiOsc osc;
    float level;
    CircularBuffer<float> delayBufferL;
    CircularBuffer<float> delayBufferR;
};