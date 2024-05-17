static EAS_I32 WT_UpdatePhaseInc (S_WT_VOICE *pWTVoice, const S_ARTICULATION *pArt, S_SYNTH_CHANNEL *pChannel, EAS_I32 pitchCents)
{
    EAS_I32 temp;

  
    temp = MULT_EG1_EG1(DEFAULT_LFO_MOD_WHEEL_TO_PITCH_CENTS,
 ((pChannel->modWheel) << (NUM_EG1_FRAC_BITS -7)));

  
    temp += MULT_EG1_EG1(DEFAULT_LFO_CHANNEL_PRESSURE_TO_PITCH_CENTS,
 ((pChannel->channelPressure) << (NUM_EG1_FRAC_BITS -7)));

  
    temp = MULT_EG1_EG1(pWTVoice->modLFO.lfoValue, temp);

  
    temp += pitchCents +
 (MULT_EG1_EG1(pWTVoice->eg2Value, pArt->eg2ToPitch)) +
 (MULT_EG1_EG1(pWTVoice->modLFO.lfoValue, pArt->lfoToPitch));

  
 return EAS_Calculate2toX(temp);
}
