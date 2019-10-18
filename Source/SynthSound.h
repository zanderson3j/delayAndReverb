/*
  ==============================================================================

    SynthSound.h
    Created: 17 Oct 2019 10:00:56pm
    Author:  Zachary Anderson

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class SynthSound : public SynthesiserSound
{
public:
    
    bool appliesToNote (int midiNoteNumber) override {
        return true;
    }
    
    bool appliesToChannel (int channelNumber) override {
        return true;
    }
};