static EAS_U8 ConvertQ (EAS_I32 q)
{

  
 if (q <= 0)
 return 0;

  
  
    q = (FILTER_Q_CONVERSION_FACTOR * q + 0x4000) >> 15;

  
 if (q >= FILTER_RESONANCE_NUM_ENTRIES)
        q = FILTER_RESONANCE_NUM_ENTRIES - 1;
 return (EAS_U8) q;
}
