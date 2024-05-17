static EAS_I16 ConvertLFOPhaseIncrement (EAS_I32 pitchCents)
{

  
 if (pitchCents > MAX_LFO_FREQUENCY_IN_PITCHCENTS)
        pitchCents = MAX_LFO_FREQUENCY_IN_PITCHCENTS;
 if (pitchCents < MIN_LFO_FREQUENCY_IN_PITCHCENTS)
        pitchCents = MIN_LFO_FREQUENCY_IN_PITCHCENTS;

  
    pitchCents = pitchCents - dlsLFOFrequencyConvert;

  
 return (EAS_I16) EAS_Calculate2toX(pitchCents);
}
