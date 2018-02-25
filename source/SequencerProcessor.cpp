/*
  ==============================================================================

    SequencerProcessor.cpp
    Created: 25 Feb 2018 12:41:20pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#include "SequencerProcessor.h"

SequencerProcessor::SequencerProcessor ( MonosynthPluginAudioProcessor& p, SequencerState& st ) : processor(p), seqState(st), startTime(Time::getMillisecondCounter())
{
    seqState.addListener(this);
}

SequencerProcessor::~SequencerProcessor()
{
    seqState.removeListener(this);
}

void SequencerProcessor::handleSequencerNoteOn(SequencerState*, int midiChannel, int midiNoteNumber, float velocity)
{
    //if (isActive)
    {
        stepCount = 0;
        lastNotePlayed = midiNoteNumber;
        currentMidiChannel = midiChannel;
        
        startPulseClock();
        
        isPlaying = true;
    }
}


void SequencerProcessor::handleSequencerNoteOff(SequencerState*, int midiChannel, int midiNoteNumber, float velocity)
{
    //if(isActive)
    {
        isPlaying = false;
    }
}


void SequencerProcessor::processSequencer(int bufferSize)
{
    int numSamples = bufferSize;
    
    while (--numSamples >= 0)
    {
        if(isPlaying)
        {
            pulseClock.update();
            
            if(pulseClock.isPulseHigh())
            {
                playStep(stepCount);
            }
        }
        
        //CHECK IF NOTES SHOULD BE RELEASED
        for (int i = 0; i < numSteps; i++)
        {
            
            if (!step[i].isReleased)
            {
                int currentTime = static_cast<int>( std::round(Time::getMillisecondCounterHiRes() ) - startTime);
                int range = 4;
                
                if (step[i].timeStamp + step[i].noteLengthMillis > currentTime - range && step[i].timeStamp + step[i].noteLengthMillis < currentTime + range )
                {
                    int note = step[i].noteNumber;
                    seqState.noteOff(currentMidiChannel, note, 1.0f);
                    step[i].isReleased = true;
                    step[i].isActive = false;
                    
                    
                    //trigger Listener
                    processor.handleNoteOff(nullptr, currentMidiChannel, note, 1.0f);
                }
            }
        }
    }
}

void SequencerProcessor::playStep(int currentStep)
{
    int newNote = lastNotePlayed + *processor.stepPitchParam[currentStep];
    int pulseInterval = (1 / pulseClock.getFrequency()) * 1000;
    int releaseTime = std::round(  *processor.stepNoteLengthParam * pulseInterval );
    
    
    //fill struct
    step[currentStep].stepNumber = currentStep;
    step[currentStep].noteNumber = newNote;
    step[currentStep].noteLengthMillis = releaseTime;
    step[currentStep].timeStamp = static_cast<int> ( std::round( Time::getMillisecondCounterHiRes() ) - startTime);
    step[currentStep].isReleased = false;
    step[currentStep].isActive = true;
    
    
    //send noteOn message
    seqState.noteOn(currentMidiChannel, newNote, 1.0f);
    
    
    
    //trigger Listener
    processor.handleNoteOn(nullptr, currentMidiChannel, newNote, 1.0f);
    
    stepCount++;
    
    if (stepCount >= numSteps)
        stepCount = 0;
}


void SequencerProcessor::startPulseClock()
{
    
    int division = (int) std::round( (double)*processor.stepDivisionFloatParam );
    double pulseTimeHz = getPulseInHz(processor.lastPosInfo, division);
    
    
    pulseClock.setSampleRate(processor.getSampleRate());
    pulseClock.setFrequency(pulseTimeHz);
    pulseClock.resetModulo();
}


double SequencerProcessor::getPulseInHz(AudioPlayHead::CurrentPositionInfo posInfo, int division)
{
    /*
     int BPM = 120;
     
     BPM = posInfo.bpm;
     const int denominator = posInfo.timeSigDenominator;
     const int msPerBeat = 60000 / BPM;
     const int msPerNote = msPerBeat * denominator / division;
     
     return msPerNote;
     */
    
    double beats_per_minute = 120;
    beats_per_minute = posInfo.bpm;
    
    const double seconds_per_beat = 60.0 / beats_per_minute;
    const double seconds_per_note = seconds_per_beat * posInfo.timeSigDenominator / division;
    
    // double seconds_per_measure = seconds_per_beat * lastPosInfo.timeSigNumerator;
    
    return 1.0 / seconds_per_note;
    
}
