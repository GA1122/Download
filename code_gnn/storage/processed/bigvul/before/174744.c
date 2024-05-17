static EAS_RESULT WT_StartVoice (S_VOICE_MGR *pVoiceMgr, S_SYNTH *pSynth, S_SYNTH_VOICE *pVoice, EAS_I32 voiceNum, EAS_U16 regionIndex)
{
    S_WT_VOICE *pWTVoice;
 const S_WT_REGION *pRegion;
 const S_ARTICULATION *pArt;
    S_SYNTH_CHANNEL *pChannel;

#if (NUM_OUTPUT_CHANNELS == 2)
    EAS_INT pan;
#endif

#ifdef EAS_SPLIT_WT_SYNTH
    S_WT_CONFIG wtConfig;
#endif

  
    pVoice->regionIndex = regionIndex;
    pVoice->voiceFlags = VOICE_FLAG_NO_SAMPLES_SYNTHESIZED_YET;

  
    pWTVoice = &pVoiceMgr->wtVoices[voiceNum];
    pChannel = &pSynth->channels[pVoice->channel & 15];

  
 if (pChannel->channelFlags & CHANNEL_FLAG_UPDATE_CHANNEL_PARAMETERS)
        WT_UpdateChannel(pVoiceMgr, pSynth, pVoice->channel & 15);

#ifdef DLS_SYNTHESIZER
 if (pVoice->regionIndex & FLAG_RGN_IDX_DLS_SYNTH)
 return DLS_StartVoice(pVoiceMgr, pSynth, pVoice, voiceNum, regionIndex);
#endif

    pRegion = &(pSynth->pEAS->pWTRegions[regionIndex]);
    pWTVoice->artIndex = pRegion->artIndex;

#ifdef _DEBUG_SYNTH
 {   }
#endif

    pArt = &pSynth->pEAS->pArticulations[pWTVoice->artIndex];

  
    pWTVoice->eg1State = eEnvelopeStateAttack;
    pWTVoice->eg1Value = 0;
    pWTVoice->eg1Increment = pArt->eg1.attackTime;
    pWTVoice->eg2State = eEnvelopeStateAttack;
    pWTVoice->eg2Value = 0;
    pWTVoice->eg2Increment = pArt->eg2.attackTime;

  
    pWTVoice->modLFO.lfoValue = 0;
    pWTVoice->modLFO.lfoPhase = -pArt->lfoDelay;

    pVoice->gain = 0;

#if (NUM_OUTPUT_CHANNELS == 2)
  
    pan = (EAS_INT) pSynth->channels[pVoice->channel & 15].pan - 64;
    pan += pArt->pan;
    EAS_CalcPanControl(pan, &pWTVoice->gainLeft, &pWTVoice->gainRight);
#endif

#ifdef _FILTER_ENABLED
  
    pWTVoice->filter.z1 = 0;
    pWTVoice->filter.z2 = 0;
#endif

  
 if (pRegion->region.keyGroupAndFlags & REGION_FLAG_USE_WAVE_GENERATOR)
 {
        pWTVoice->phaseAccum = 4574296;
        pWTVoice->loopStart = WT_NOISE_GENERATOR;
        pWTVoice->loopEnd = 4574295;
 }

  
 else
 {

#ifdef EAS_SPLIT_WT_SYNTH
 if (voiceNum < NUM_PRIMARY_VOICES)
            pWTVoice->phaseAccum = (EAS_U32) pSynth->pEAS->pSamples + pSynth->pEAS->pSampleOffsets[pRegion->waveIndex];
 else
            pWTVoice->phaseAccum = pSynth->pEAS->pSampleOffsets[pRegion->waveIndex];
#else
        pWTVoice->phaseAccum = (EAS_U32) pSynth->pEAS->pSamples + pSynth->pEAS->pSampleOffsets[pRegion->waveIndex];
#endif

 if (pRegion->region.keyGroupAndFlags & REGION_FLAG_IS_LOOPED)
 {
            pWTVoice->loopStart = pWTVoice->phaseAccum + pRegion->loopStart;
            pWTVoice->loopEnd = pWTVoice->phaseAccum + pRegion->loopEnd - 1;
 }
 else
            pWTVoice->loopStart = pWTVoice->loopEnd = pWTVoice->phaseAccum + pSynth->pEAS->pSampleLen[pRegion->waveIndex] - 1;
 }

#ifdef EAS_SPLIT_WT_SYNTH
  
 if (voiceNum >= NUM_PRIMARY_VOICES)
 {
        wtConfig.phaseAccum = pWTVoice->phaseAccum;
        wtConfig.loopStart = pWTVoice->loopStart;
        wtConfig.loopEnd = pWTVoice->loopEnd;
        wtConfig.gain = pVoice->gain;

#if (NUM_OUTPUT_CHANNELS == 2)
        wtConfig.gainLeft = pWTVoice->gainLeft;
        wtConfig.gainRight = pWTVoice->gainRight;
#endif

        WTE_ConfigVoice(voiceNum - NUM_PRIMARY_VOICES, &wtConfig, pVoiceMgr->pFrameBuffer);
 }
#endif

 return EAS_SUCCESS;
}
