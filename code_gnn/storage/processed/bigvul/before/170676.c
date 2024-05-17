EAS_I16 ConvertDelay (EAS_I32 timeCents)
{
    EAS_I32 temp;

 if (timeCents == ZERO_TIME_IN_CENTS)
 return 0;

  
    temp = timeCents - dlsRateConvert;

  
    temp = FMUL_15x15(temp, TIME_CENTS_TO_LOG2);

  
    temp = EAS_LogToLinear16(temp - (15 << 10));

 if (temp < SYNTH_FULL_SCALE_EG1_GAIN)
 return (EAS_I16) temp;
 return SYNTH_FULL_SCALE_EG1_GAIN;
}
