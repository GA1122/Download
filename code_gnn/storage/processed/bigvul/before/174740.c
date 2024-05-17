static EAS_RESULT WT_Initialize (S_VOICE_MGR *pVoiceMgr)
{
    EAS_INT i;

 for (i = 0; i < NUM_WT_VOICES; i++)
 {

        pVoiceMgr->wtVoices[i].artIndex = DEFAULT_ARTICULATION_INDEX;

        pVoiceMgr->wtVoices[i].eg1State = DEFAULT_EG1_STATE;
        pVoiceMgr->wtVoices[i].eg1Value = DEFAULT_EG1_VALUE;
        pVoiceMgr->wtVoices[i].eg1Increment = DEFAULT_EG1_INCREMENT;

        pVoiceMgr->wtVoices[i].eg2State = DEFAULT_EG2_STATE;
        pVoiceMgr->wtVoices[i].eg2Value = DEFAULT_EG2_VALUE;
        pVoiceMgr->wtVoices[i].eg2Increment = DEFAULT_EG2_INCREMENT;

  
#if (NUM_OUTPUT_CHANNELS == 2)
        pVoiceMgr->wtVoices[i].gainLeft = DEFAULT_VOICE_GAIN;
        pVoiceMgr->wtVoices[i].gainRight = DEFAULT_VOICE_GAIN;
#endif

        pVoiceMgr->wtVoices[i].phaseFrac = DEFAULT_PHASE_FRAC;
        pVoiceMgr->wtVoices[i].phaseAccum = DEFAULT_PHASE_INT;

#ifdef _FILTER_ENABLED
        pVoiceMgr->wtVoices[i].filter.z1 = DEFAULT_FILTER_ZERO;
        pVoiceMgr->wtVoices[i].filter.z2 = DEFAULT_FILTER_ZERO;
#endif
 }

 return EAS_TRUE;
}
