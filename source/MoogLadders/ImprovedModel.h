/*
 Copyright 2012 Stefano D'Angelo <zanga.mail@gmail.com>
 
 Permission to use, copy, modify, and/or distribute this software for any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.
 
 THIS SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#ifndef IMPROVED_LADDER_H
#define IMPROVED_LADDER_H

#include "LadderFilterBase.h"
//#include "../../JuceLibraryCode/JuceHeader.h"

/*
 This model is based on a reference implementation of an algorithm developed by
 Stefano D'Angelo and Vesa Valimaki, presented in a paper published at ICASSP in 2013.
 This improved model is based on a circuit analysis and compared against a refddderence
 Ngspice simulation. In the paper, it is noted that this particular model is
 more accurate in preserving the self-oscillating nature of the real filter.
 
 References: "An Improved Virtual Analog Model of the Moog Ladder Filter"
 Original Implementation: D'Angelo, Valimaki
 */

// Thermal voltage (26 milliwats at room temperature)
#define VT 0.312


class ImprovedMoog : public LadderFilterBase
{
public:
    
    ImprovedMoog() : LadderFilterBase(), sampleRate(44100.0)
    {
        drive = 1.0f;
        
        Reset();
    }
    
    virtual ~ImprovedMoog()
    {
        
    }
    virtual void Reset() override
    {
        for (int i = 0; i < 4; i++)
        {
            V[i] = 0;
            dV[i] = 0;
            tV[i] = 0;
        }
        
    }
    
    virtual void Update() override
    {
        x = (MOOG_PI * cutoff.get()) / (sampleRate * multiplier);
        g = 4.0 * MOOG_PI * VT * cutoff.get() * (1.0 - x) / (1.0 + x);
    }
    
    virtual void Process(float* samples, size_t n) noexcept override
    {
        renderBlock(samples, n);
        
    }
    
    virtual void Process(double* samples, size_t n) noexcept override
    {
        renderBlock(samples, n);
    }
    
    
    
    virtual void SetSampleRate (double sr) override
    {
        sampleRate = sr;
        multiplier =  jmin(1.0, 96000.0 / sr);
    }
    
    virtual void SetResonance(double r) override
    {
        resonance = r * 4.0;
    }
    
    virtual void SetCutoff(double c) override
    {
        cutoff.set(c);
        Update();
    }
    
    virtual void SetDrive (double d ) override
    {
        drive = d;
    }
    
 
 
    
private:
    
    template <typename FloatType>
    void renderBlock(FloatType* samples, size_t n)
    {
        
        FloatType dV0, dV1, dV2, dV3;
        
        for (uint32_t i = 0; i < n; i++)
        {
            
            dV0 = -g * (fast_tanh((drive * samples[i] + resonance.get() * V[3]) / (2.0 * VT)) + tV[0]);
            V[0] += (dV0 + dV[0]) / (2.0 * sampleRate * multiplier);
            dV[0] = dV0;
            tV[0] = fast_tanh(V[0] / (2.0 * VT));
            
            dV1 = g * (tV[0] - tV[1]);
            V[1] += (dV1 + dV[1]) / (2.0 * sampleRate * multiplier);
            dV[1] = dV1;
            tV[1] = fast_tanh(V[1] / (2.0 * VT));
            
            dV2 = g * (tV[1] - tV[2]);
            V[2] += (dV2 + dV[2]) / (2.0 * sampleRate* multiplier);
            dV[2] = dV2;
            tV[2] = fast_tanh(V[2] / (2.0 * VT));
            
            dV3 = g * (tV[2] - tV[3]);
            V[3] += (dV3 + dV[3]) / (2.0 * sampleRate * multiplier);
            dV[3] = dV3;
            tV[3] = fast_tanh(V[3] / (2.0 * VT));
            
            samples[i] = V[3];
            
        }
    }
    
    double V[4];
    double dV[4];
    double tV[4];
    
    double x;
    double g;
    
    double sampleRate;
    double multiplier;
};

#endif
