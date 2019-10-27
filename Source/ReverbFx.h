/*
  ==============================================================================

    ReverbFx.h
    Created: 23 Oct 2019 10:32:39pm
    Author:  Zachary Anderson

  ==============================================================================
*/

#pragma once

#include "fxobjects.h"

class ReverbFx
{
public:
    ReverbFx ()
    {
        combBuffer1.createCircularBuffer(88200);
        combBuffer1.flushBuffer();
        
        combBuffer2.createCircularBuffer(88200);
        combBuffer2.flushBuffer();
        
        combBuffer3.createCircularBuffer(88200);
        combBuffer3.flushBuffer();
        
        combBuffer4.createCircularBuffer(88200);
        combBuffer4.flushBuffer();
        
        allBuffer1.createCircularBuffer(88200);
        allBuffer1.flushBuffer();
        
        allBuffer2.createCircularBuffer(88200);
        allBuffer2.flushBuffer();
        
        delaySamples1 = (int) 16.0*44.1;
        delaySamples2 = (int) 8.0*44.1;
        delaySamples3 = (int) 4.0*44.1;
        delaySamples4 = (int) 2.0*44.1;
        delaySamples5 = (int) 42.6*44.1;
        delaySamples6 = (int) 64.0*44.1;
        
        yn1 = 0.0;
        yn2 = 0.0;
        
        
    }
    
    void effect (AudioBuffer<float> &outputBuffer, int sample)
    {
        float wn1 = combBuffer1.readBuffer(delaySamples1);
        float wn2 = combBuffer2.readBuffer(delaySamples2);
        float wn3 = combBuffer3.readBuffer(delaySamples3);
        float wn4 = combBuffer4.readBuffer(delaySamples4);
        
        float dn1 = outputBuffer.getSample(0, sample)*0.3 + (0.3*wn1);
        float dn2 = outputBuffer.getSample(0, sample)*0.3 + (0.4*wn2);
        float dn3 = outputBuffer.getSample(0, sample)*0.3 + (0.5*wn3);
        float dn4 = outputBuffer.getSample(0, sample)*0.3 + (0.6*wn4);
        
        combBuffer1.writeBuffer(dn1);
        combBuffer2.writeBuffer(dn2);
        combBuffer3.writeBuffer(dn3);
        combBuffer4.writeBuffer(dn4);
        
        float combMix = dn1 + dn2 + dn3 + dn4;
        
        float vn1 = allBuffer1.readBuffer(delaySamples5);
        float un1 = combMix*0.3 + (0.7*vn1);
        
        allBuffer1.writeBuffer(un1);
        
        yn1 = (-0.7*combMix) + un1 + (0.7*yn1);
        
        
        float vn2 = allBuffer2.readBuffer(delaySamples5);
        float un2 = yn1*0.3 + (0.7*vn2);
        
        allBuffer2.writeBuffer(un2);
        
        yn2 = (-0.7*yn1) + un2 + (0.7*yn1);
        
        //yn2 = filter.lopass(yn2, 3000.0);
        
        outputBuffer.addSample(0, sample, yn2 * 0.125);
        outputBuffer.addSample(1, sample, yn2 * 0.125);
    }
    
    
    
private:
    CircularBuffer<float> combBuffer1;
    CircularBuffer<float> combBuffer2;
    CircularBuffer<float> combBuffer3;
    CircularBuffer<float> combBuffer4;
    CircularBuffer<float> allBuffer1;
    CircularBuffer<float> allBuffer2;
    int delaySamples1;
    int delaySamples2;
    int delaySamples3;
    int delaySamples4;
    int delaySamples5;
    int delaySamples6;
    float yn1;
    float yn2;
    maxiFilter filter;
};