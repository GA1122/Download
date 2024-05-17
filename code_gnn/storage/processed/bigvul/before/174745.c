static void WT_SustainPedal (S_VOICE_MGR *pVoiceMgr, S_SYNTH *pSynth, S_SYNTH_VOICE *pVoice, S_SYNTH_CHANNEL *pChannel, EAS_I32 voiceNum)
{
    S_WT_VOICE *pWTVoice;

#ifdef DLS_SYNTHESIZER
 if (pVoice->regionIndex & FLAG_RGN_IDX_DLS_SYNTH)
 {
        DLS_SustainPedal(pVoiceMgr, pSynth, pVoice, pChannel, voiceNum);
 return;
 }
#endif

  
    pWTVoice = &pVoiceMgr->wtVoices[voiceNum];
 if (pWTVoice->eg1Value < pSynth->pEAS->pArticulations[pWTVoice->artIndex].eg1.sustainLevel)
 return;

  
  
    pWTVoice->eg1State = eEnvelopeStateDecay;
    pVoice->voiceState = eVoiceStatePlay;

  
    pVoice->voiceFlags |= VOICE_FLAG_SUSTAIN_PEDAL_DEFER_NOTE_OFF;

#ifdef _DEBUG_SYNTH
 {   }
#endif
}
