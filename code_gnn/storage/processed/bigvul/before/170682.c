static EAS_I16 ConvertSustain (EAS_I32 sustain)
{
  
 if (sustain == 0)
 return 0;

  
  
    sustain = (sustain * SUSTAIN_LINEAR_CONVERSION_FACTOR) >> 15;

 if (sustain > SYNTH_FULL_SCALE_EG1_GAIN)
 return SYNTH_FULL_SCALE_EG1_GAIN;
 return (EAS_I16) sustain;
}
