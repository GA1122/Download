static EAS_I32 WT_UpdateGain (S_SYNTH_VOICE *pVoice, S_WT_VOICE *pWTVoice, const S_ARTICULATION *pArt, S_SYNTH_CHANNEL *pChannel, EAS_I32 gain)
{
    EAS_I32 lfoGain;
    EAS_I32 temp;

  
  
    temp = (pVoice->velocity) << (NUM_EG1_FRAC_BITS - 7);
    temp = MULT_EG1_EG1(temp, temp);

  
    temp = MULT_EG1_EG1(temp, gain);

  
    temp = MULT_EG1_EG1(temp, pChannel->staticGain);

  
    lfoGain = MULT_EG1_EG1(pWTVoice->modLFO.lfoValue, pArt->lfoToGain);
    lfoGain = MULT_EG1_EG1(lfoGain, LFO_GAIN_TO_CENTS);

  
    lfoGain = EAS_Calculate2toX(lfoGain);
    temp = MULT_EG1_EG1(temp, lfoGain);

  
    temp = (EAS_I16)MULT_EG1_EG1(temp, pWTVoice->eg1Value);

 return temp;
}
