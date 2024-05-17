static void WT_UpdateFilter (S_WT_VOICE *pWTVoice, S_WT_INT_FRAME *pIntFrame, const S_ARTICULATION *pArt)
{
    EAS_I32 cutoff;

  
 if (pArt->filterCutoff == DEFAULT_EAS_FILTER_CUTOFF_FREQUENCY)
 {
        pIntFrame->frame.k = 0;
 return;
 }

  
    cutoff = MULT_EG1_EG1(pWTVoice->eg2Value, pArt->eg2ToFc);
    cutoff += pArt->filterCutoff;

  
    cutoff -= FILTER_CUTOFF_FREQ_ADJUST + A5_PITCH_OFFSET_IN_CENTS;

  
 if (cutoff > FILTER_CUTOFF_MAX_PITCH_CENTS)
        cutoff = FILTER_CUTOFF_MAX_PITCH_CENTS;
 else if (cutoff < FILTER_CUTOFF_MIN_PITCH_CENTS)
        cutoff = FILTER_CUTOFF_MIN_PITCH_CENTS;

    WT_SetFilterCoeffs(pIntFrame, cutoff, pArt->filterQ);
}
