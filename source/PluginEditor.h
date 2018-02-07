/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/** This is the editor component that our filter will display.
*/
class MonosynthPluginAudioProcessorEditor  : public AudioProcessorEditor,
										
                                            private Timer
                                            //private MidiKeyboardStateListener
{
public:
    MonosynthPluginAudioProcessorEditor (MonosynthPluginAudioProcessor&);
    ~MonosynthPluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;

	// Binary resources:
	static const char* oscSquareWaveSymbol_svg;
	static const int oscSquareWaveSymbol_svgSize;
	static const char* oscSquareWaveSymbol_svg2;
	static const int oscSquareWaveSymbol_svg2Size;
	static const char* oscSawWaveSymbol_svg;
	static const int oscSawWaveSymbol_svgSize;
	static const char* oscSineWaveSymbol_svg;
	static const int oscSineWaveSymbol_svgSize;
	static const char* attackCurveLinear_symbol_svg;
	static const int attackCurveLinear_symbol_svgSize;
	static const char* attackCurveExponential_symbol_svg;
	static const int attackCurveExponential_symbol_svgSize;
	static const char* decayCurveLinear_symbol_svg;
	static const int decayCurveLinear_symbol_svgSize;
	static const char* decayCurveExponential_symbol_svg;
	static const int decayCurveExponential_symbol_svgSize;

private:
    class ParameterSlider;

    LinearSmoothedValue<float> newValueSmooth;
    
    
    Colour lineColour;
    
    MidiKeyboardComponent midiKeyboard;

	Label timecodeDisplayLabel;

   ScopedPointer<Label>  
        mainLabel,
        titleLabel,
        osc1GainLabel,
        osc1WaveformLabel,
        osc1OffsetLabel,
        oscillatorsLabel,
        osc1TuneLabel,
        osc2GainLabel,
        osc2WaveformLabel,
        osc2OffsetLabel,
        osc2TuneLabel,
        osc3GainLabel,
        osc3WaveformLabel,
        osc3OffsetLabel,
        osc3TuneLabel,
    
        oscSyncLabel,
        oscSyncONLabel,
        oscSyncOFFLabel,
        filterLabel,
        filterCutoffLabel,
        filterResonanceLabel,
        filterContourLabel,
        filterDriveLabel,
    
        filterMoogLabel,
        filterMS20Label,
        filter303Label,
    
        envelopesLabel,
        envAmpLabel,
        envFilterLabel,
        envPitchLabel,
        volumeLabel,
        pitchModLabel,
    
        lfoLabel,
    
        lfoRateLabel,
        lfoModeLabel,
        lfoIntensityLabel,
        modTargetLabel,
        modTargetCutoffLabel,
        modTargetPitchLabel,
        modTargetOffLabel,
    
    saturationLabel

            ;
    
    ScopedPointer<ParameterSlider> 
        volumeSlider,
        osc1GainSlider,
        osc1OffsetSlider,
        osc1TuneSlider,
        osc1WaveformSlider,

        osc2GainSlider,
        osc2OffsetSlider,
        osc2TuneSlider,
        osc2WaveformSlider,

        osc3GainSlider,
        osc3OffsetSlider,
        osc3TuneSlider,
        osc3WaveformSlider,

        pitchModSlider,

        filterCutoffSlider, 
        filterResonanceSlider,
        filterContourSlider,
        filterDriveSlider,

        attackSlider1,
        decaySlider1,
        sustainSlider1,
        releaseSlider1,
        attackCurveSlider1,
        decRelCurveSlider1,


        attackSlider2,
        decaySlider2,
        sustainSlider2,
        releaseSlider2,
        attackCurveSlider2,
        decRelCurveSlider2,

        attackSlider3,
        decaySlider3,
        sustainSlider3,
        releaseSlider3,
        attackCurveSlider3,
        decRelCurveSlider3,

        modTargetSlider,

        lfoRateSlider,
        lfoModeSlider,
        lfoIntensitySlider,
		lfoSyncSlider,
    
        filterSelectSlider,
    
        lfoSyncedFreqSlider,
    
        oscSyncSlider,
    
        filterOrderSlider,
    
        saturationSwitchSlider,
    
        pulsewidthAmount1Slider,
        pulsewidthAmount2Slider,
        pulsewidthAmount3Slider,

		saturationSlider
        ;

    std::unique_ptr<Drawable> drawable1;
    std::unique_ptr<Drawable> drawable2;
    std::unique_ptr<Drawable> drawable3;
    std::unique_ptr<Drawable> drawable4;
    std::unique_ptr<Drawable> drawable5;
    std::unique_ptr<Drawable> drawable6;
    std::unique_ptr<Drawable> drawable7;
    std::unique_ptr<Drawable> drawable8;
    std::unique_ptr<Drawable> drawable9;
    std::unique_ptr<Drawable> drawable10;
    std::unique_ptr<Drawable> drawable11;
    std::unique_ptr<Drawable> drawable12;
    std::unique_ptr<Drawable> drawable13;
    std::unique_ptr<Drawable> drawable14;
    std::unique_ptr<Drawable> drawable15;
    std::unique_ptr<Drawable> drawable16;
    std::unique_ptr<Drawable> drawable17;
    std::unique_ptr<Drawable> drawable18;
    std::unique_ptr<Drawable> drawable19;
    std::unique_ptr<Drawable> drawable20;
    std::unique_ptr<Drawable> drawable21;
    std::unique_ptr<Drawable> drawable22;
    std::unique_ptr<Drawable> drawable23;
    std::unique_ptr<Drawable> drawable24;
    
    
    String font;
    //==============================================================================
    MonosynthPluginAudioProcessor& getProcessor() const
    {
        return static_cast<MonosynthPluginAudioProcessor&> (processor);
    }

    void updateTimecodeDisplay (AudioPlayHead::CurrentPositionInfo);
};
