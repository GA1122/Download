static void WT_ReleaseVoice (S_VOICE_MGR *pVoiceMgr, S_SYNTH *pSynth, S_SYNTH_VOICE *pVoice, EAS_I32 voiceNum)
{
    S_WT_VOICE *pWTVoice;
 const S_ARTICULATION *pArticulation;

#ifdef DLS_SYNTHESIZER
 if (pVoice->regionIndex & FLAG_RGN_IDX_DLS_SYNTH)
 {
        DLS_ReleaseVoice(pVoiceMgr, pSynth, pVoice, voiceNum);
 return;
 }
#endif

    pWTVoice = &pVoiceMgr->wtVoices[voiceNum];
    pArticulation = &pSynth->pEAS->pArticulations[pWTVoice->artIndex];

  
    pWTVoice->eg1State = eEnvelopeStateRelease;
    pWTVoice->eg1Increment = pArticulation->eg1.releaseTime;

  

  
    pWTVoice->eg2State = eEnvelopeStateRelease;
    pWTVoice->eg2Increment = pArticulation->eg2.releaseTime;
}
