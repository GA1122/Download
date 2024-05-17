static void WT_MuteVoice (S_VOICE_MGR *pVoiceMgr, S_SYNTH *pSynth, S_SYNTH_VOICE *pVoice, EAS_I32 voiceNum)
{

#ifdef DLS_SYNTHESIZER
 if (pVoice->regionIndex & FLAG_RGN_IDX_DLS_SYNTH)
 {
        DLS_MuteVoice(pVoiceMgr, pSynth, pVoice, voiceNum);
 return;
 }
#endif

  
    pVoice->voiceFlags &=
 ~(VOICE_FLAG_DEFER_MIDI_NOTE_OFF |
        VOICE_FLAG_SUSTAIN_PEDAL_DEFER_NOTE_OFF |
        VOICE_FLAG_DEFER_MUTE);

  
    pVoiceMgr->wtVoices[voiceNum].eg1State = eEnvelopeStateMuted;
    pVoiceMgr->wtVoices[voiceNum].eg2State = eEnvelopeStateMuted;
}
