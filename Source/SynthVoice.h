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
#include "ReverbFx.h"

class SynthVoice : public SynthesiserVoice
{
public:
    
    SynthVoice()
    {
        env.setAttack(1000);
        env.setDecay(250);
        env.setSustain(0.8);
        env.setRelease(500);
        
        reverbParameters.roomSize   = 0.1f;     /**< Room size, 0 to 1.0, where 1.0 is big, 0 is small. */
        reverbParameters.damping    = 0.9f;     /**< Damping, 0 to 1.0, where 0 is not damped, 1.0 is fully damped. */
        reverbParameters.wetLevel   = 0.22f;    /**< Wet level, 0 to 1.0 */
        reverbParameters.dryLevel   = 0.43f;     /**< Dry level, 0 to 1.0 */
        reverbParameters.width      = 0.2f;     /**< Reverb width, 0 to 1.0, where 1.0 is very wide. */
        reverbParameters.freezeMode = 0.0f;     /**< Freeze mode - values < 0.5 are "normal" mode, values > 0.5
                                          put the reverb into a continuous feedback loop. */
//        reverb.setParameters(reverbParameters);
        
        reverbMilis = 0.0;
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
            double sound = env.adsr(sineWave, env.trigger) * level * 0.125f;
            //sound = distortion.atanDist(sound, 200);
            
            outputBuffer.addSample(0, startSample + sample, sound);
            outputBuffer.addSample(1, startSample + sample, sound);
            
            if(reverbMilis > 100.0) {
                reverbFx.effect(outputBuffer, startSample + sample);
            }
            
            
            delayFx.effect(outputBuffer, startSample + sample);
        }
        
//        reverb.processStereo(outputBuffer.getWritePointer(0, startSample),
//                             outputBuffer.getWritePointer(1, startSample),
//                             numSamples);
    }
    
    void setDelaySamples(int delaySamples)
    {
        delayFx.setDelaySamples(delaySamples);
    }
    
    void setReverbMilis(int reverbMilis)
    {
        //
        this->reverbMilis = reverbMilis;
    }
    
private:
    double frequency;
    maxiOsc osc;
    maxiEnv env;
    float level;
    DelayFx delayFx;
    Reverb reverb;
    Reverb::Parameters reverbParameters;
    maxiDistortion distortion;
    ReverbFx reverbFx;
    double reverbMilis;
};