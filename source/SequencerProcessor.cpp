/*
  ==============================================================================

    SequencerProcessor.cpp
    Created: 25 Feb 2018 12:41:20pm
    Author:  Sander Vermeer

  ==============================================================================
*/

#include "SequencerProcessor.h"

SequencerProcessor::SequencerProcessor ( MidiKeyboardState& ks ) : 
	keyState(ks), 
	lastNotePlayed(60), 
	currentMidiChannel(1),
	stepCount(0),
	maxSteps(8),
	isPlaying(false),
	isActive(false), 
	stepPitchValue {0,0,0,0,0,0,0,0},
	noteLength(0.5),
	timeDivision(16.0),
	globalSampleCount(0),
	sampleRate(44100.0)
{
    keyState.addListener(this);
}

SequencerProcessor::~SequencerProcessor()
{
    keyState.removeListener(this);
}

void SequencerProcessor::handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
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


void SequencerProcessor::handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    //if(isActive)
    {
        isPlaying = false;
    }
}


void SequencerProcessor::processSequencer(MidiBuffer& midBuf, int bufferSize, bool useSequencer)
{
    int numSamples = bufferSize;
    int sampleCount = 0;
   
    if (useSequencer)
    {
        midBuf.clear();
        
        while (--numSamples >= 0)
        {
            if(isPlaying)
            {
                pulseClock.update();
                
                if(pulseClock.isPulseHigh())
                {
                    playStep(midBuf, stepCount, sampleCount);
                }
            }
            
            //CHECK IF NOTES SHOULD BE RELEASED
            for (int i = 0; i <= maxSteps; i++) 
            {
                
                if (!step[i].isReleased)
                {
                    unsigned long int currentTime = globalSampleCount;
                    
                    if (step[i].timeStamp + step[i].noteLengthTicks < currentTime)
                    {
                        int note = step[i].noteNumber;
                        midBuf.addEvent( MidiMessage::noteOff(currentMidiChannel, note, 1.0f), sampleCount);
                        step[i].isReleased = true;
                        step[i].isActive = false;
                    }
                }
            }
            sampleCount++;
			globalSampleCount = (globalSampleCount + 1) % std::numeric_limits<unsigned long int>::max();
        }
    }
}

void SequencerProcessor::playStep(MidiBuffer& midBuf, int currentStep, int curSample)
{
    int newNote = lastNotePlayed + stepPitchValue[currentStep];
    double pulseIntervalSamples = getPulseInSamples(currentBPM, timeDivision, sampleRate);
    uint32 releaseTime = std::round( noteLength * pulseIntervalSamples );
    
   
    
    //fill struct
    step[currentStep].stepNumber = currentStep;
    step[currentStep].noteNumber = newNote;
    step[currentStep].noteLengthTicks = releaseTime;
    step[currentStep].timeStamp = (uint32)globalSampleCount;
    step[currentStep].isReleased = false;
    step[currentStep].isActive = true;
    
    //send noteOn message
    //seqState.noteOn(currentMidiChannel, newNote, 1.0f);
    midBuf.addEvent(MidiMessage::noteOn(currentMidiChannel, newNote, 1.0f), curSample);
    
    stepCount++;
    
    if (stepCount > maxSteps)
        stepCount = 0;
}


void SequencerProcessor::startPulseClock()
{
    
    int division = (int)timeDivision;
    double pulseTimeHz = getPulseInHz(currentBPM, division);
    
    pulseClock.setFrequency(pulseTimeHz);
    pulseClock.resetModulo();
}


double SequencerProcessor::getPulseInHz(int bpm, int division)
{
    double beats_per_minute = 120;
    beats_per_minute = bpm;
    
    const double seconds_per_beat = 60.0 / beats_per_minute;
    const double seconds_per_note = seconds_per_beat * 4 / division;
    
    // double seconds_per_measure = seconds_per_beat * lastPosInfo.timeSigNumerator;
    
    return 1.0 / seconds_per_note;
}

int SequencerProcessor::getPulseInSamples(int bpm, int division, double sr )
{
    int beats_per_minute = 120;
    beats_per_minute = bpm;
    
    const int samples_per_millis = sr / 1000.0;
    const int millis_per_beat = 60000 / beats_per_minute;
    const int millis_per_note = millis_per_beat * 4 / division;
    
    // double seconds_per_measure = seconds_per_beat * lastPosInfo.timeSigNumerator;
    
    return millis_per_note * samples_per_millis;
}
