static void Convert_art (SDLS_SYNTHESIZER_DATA *pDLSData, const S_DLS_ART_VALUES *pDLSArt,  EAS_U16 artIndex)
{
    S_DLS_ARTICULATION *pArt;

  
    pArt = &pDLSData->pDLS->pDLSArticulations[artIndex];

  
    pArt->modLFO.lfoFreq = ConvertLFOPhaseIncrement(pDLSArt->values[PARAM_MOD_LFO_FREQ]);
    pArt->modLFO.lfoDelay = -ConvertDelay(pDLSArt->values[PARAM_MOD_LFO_DELAY]);
    pArt->vibLFO.lfoFreq = ConvertLFOPhaseIncrement(pDLSArt->values[PARAM_VIB_LFO_FREQ]);
    pArt->vibLFO.lfoDelay = -ConvertDelay(pDLSArt->values[PARAM_VIB_LFO_DELAY]);

  
    pArt->eg1.delayTime = ConvertDelay(pDLSArt->values[PARAM_VOL_EG_DELAY]);
    pArt->eg1.attackTime = pDLSArt->values[PARAM_VOL_EG_ATTACK];
    pArt->eg1.holdTime = pDLSArt->values[PARAM_VOL_EG_HOLD];
    pArt->eg1.decayTime = pDLSArt->values[PARAM_VOL_EG_DECAY];
    pArt->eg1.sustainLevel = ConvertSustain(pDLSArt->values[PARAM_VOL_EG_SUSTAIN]);
    pArt->eg1.releaseTime = ConvertRate(pDLSArt->values[PARAM_VOL_EG_RELEASE]);
    pArt->eg1.velToAttack = pDLSArt->values[PARAM_VOL_EG_VEL_TO_ATTACK];
    pArt->eg1.keyNumToDecay = pDLSArt->values[PARAM_VOL_EG_KEY_TO_DECAY];
    pArt->eg1.keyNumToHold = pDLSArt->values[PARAM_VOL_EG_KEY_TO_HOLD];
    pArt->eg1ShutdownTime = ConvertRate(pDLSArt->values[PARAM_VOL_EG_SHUTDOWN]);

  
    pArt->eg2.delayTime = ConvertDelay(pDLSArt->values[PARAM_MOD_EG_DELAY]);
    pArt->eg2.attackTime = pDLSArt->values[PARAM_MOD_EG_ATTACK];
    pArt->eg2.holdTime = pDLSArt->values[PARAM_MOD_EG_HOLD];
    pArt->eg2.decayTime = pDLSArt->values[PARAM_MOD_EG_DECAY];
    pArt->eg2.sustainLevel = ConvertSustain(pDLSArt->values[PARAM_MOD_EG_SUSTAIN]);
    pArt->eg2.releaseTime = ConvertRate(pDLSArt->values[PARAM_MOD_EG_RELEASE]);
    pArt->eg2.velToAttack = pDLSArt->values[PARAM_MOD_EG_VEL_TO_ATTACK];
    pArt->eg2.keyNumToDecay = pDLSArt->values[PARAM_MOD_EG_KEY_TO_DECAY];
    pArt->eg2.keyNumToHold = pDLSArt->values[PARAM_MOD_EG_KEY_TO_HOLD];

  
    pArt->filterCutoff = pDLSArt->values[PARAM_INITIAL_FC];
    pArt->filterQandFlags = ConvertQ(pDLSArt->values[PARAM_INITIAL_Q]);
    pArt->modLFOToFc = pDLSArt->values[PARAM_MOD_LFO_TO_FC];
    pArt->modLFOCC1ToFc = pDLSArt->values[PARAM_MOD_LFO_CC1_TO_FC];
    pArt->modLFOChanPressToFc = pDLSArt->values[PARAM_MOD_LFO_CHAN_PRESS_TO_FC];
    pArt->eg2ToFc = pDLSArt->values[PARAM_MOD_EG_TO_FC];
    pArt->velToFc = pDLSArt->values[PARAM_VEL_TO_FC];
    pArt->keyNumToFc = pDLSArt->values[PARAM_KEYNUM_TO_FC];

  
    pArt->modLFOToGain = pDLSArt->values[PARAM_MOD_LFO_TO_GAIN];
    pArt->modLFOCC1ToGain = pDLSArt->values[PARAM_MOD_LFO_CC1_TO_GAIN];
    pArt->modLFOChanPressToGain = pDLSArt->values[PARAM_MOD_LFO_CHAN_PRESS_TO_GAIN];

  
    pArt->tuning = pDLSArt->values[PARAM_TUNING];
    pArt->keyNumToPitch = pDLSArt->values[PARAM_KEYNUM_TO_PITCH];
    pArt->vibLFOToPitch = pDLSArt->values[PARAM_VIB_LFO_TO_PITCH];
    pArt->vibLFOCC1ToPitch = pDLSArt->values[PARAM_VIB_LFO_CC1_TO_PITCH];
    pArt->vibLFOChanPressToPitch = pDLSArt->values[PARAM_VIB_LFO_CHAN_PRESS_TO_PITCH];
    pArt->modLFOToPitch = pDLSArt->values[PARAM_MOD_LFO_TO_PITCH];
    pArt->modLFOCC1ToPitch = pDLSArt->values[PARAM_MOD_LFO_CC1_TO_PITCH];
    pArt->modLFOChanPressToPitch = pDLSArt->values[PARAM_MOD_LFO_CHAN_PRESS_TO_PITCH];
    pArt->eg2ToPitch = pDLSArt->values[PARAM_MOD_EG_TO_PITCH];

  
    pArt->pan = ConvertPan(pDLSArt->values[PARAM_DEFAULT_PAN]);

 if (pDLSArt->values[PARAM_VEL_TO_GAIN] != 0)
        pArt->filterQandFlags |= FLAG_DLS_VELOCITY_SENSITIVE;

#ifdef _REVERB
    pArt->reverbSend = pDLSArt->values[PARAM_DEFAULT_REVERB_SEND];
    pArt->cc91ToReverbSend = pDLSArt->values[PARAM_MIDI_CC91_TO_REVERB_SEND];
#endif

#ifdef _CHORUS
    pArt->chorusSend = pDLSArt->values[PARAM_DEFAULT_CHORUS_SEND];
    pArt->cc93ToChorusSend = pDLSArt->values[PARAM_MIDI_CC93_TO_CHORUS_SEND];
#endif
}
