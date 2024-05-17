void WT_SetFilterCoeffs (S_WT_INT_FRAME *pIntFrame, EAS_I32 cutoff, EAS_I32 resonance)
{
    EAS_I32 temp;

  
    cutoff = EAS_Calculate2toX(cutoff);

  
    temp = k2g1[resonance] + MULT_AUDIO_COEF(cutoff, k2g2[resonance]);
    temp = k2g0 + MULT_AUDIO_COEF(cutoff, temp);
    pIntFrame->frame.b2 = temp;

  
    temp = MULT_AUDIO_COEF(cutoff, nk1g2);
    temp = nk1g0 + MULT_AUDIO_COEF(cutoff, temp);
    temp += MULT_AUDIO_COEF(temp, pIntFrame->frame.b2);
    pIntFrame->frame.b1 = temp >> 1;

  
    temp = n1g2[resonance] + MULT_AUDIO_COEF(cutoff, n1g3[resonance]);
    temp = MULT_AUDIO_COEF(cutoff, temp);
    temp = MULT_AUDIO_COEF(cutoff, temp);
    pIntFrame->frame.k = temp;
}
