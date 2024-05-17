int Reverb_setConfig(ReverbContext *pContext, effect_config_t *pConfig){
    LVM_Fs_en   SampleRate;

    CHECK_ARG(pContext != NULL);
    CHECK_ARG(pConfig != NULL);

    CHECK_ARG(pConfig->inputCfg.samplingRate == pConfig->outputCfg.samplingRate);
    CHECK_ARG(pConfig->inputCfg.format == pConfig->outputCfg.format);
    CHECK_ARG((pContext->auxiliary && pConfig->inputCfg.channels == AUDIO_CHANNEL_OUT_MONO) ||
 ((!pContext->auxiliary) && pConfig->inputCfg.channels == AUDIO_CHANNEL_OUT_STEREO));
    CHECK_ARG(pConfig->outputCfg.channels == AUDIO_CHANNEL_OUT_STEREO);
    CHECK_ARG(pConfig->outputCfg.accessMode == EFFECT_BUFFER_ACCESS_WRITE
 || pConfig->outputCfg.accessMode == EFFECT_BUFFER_ACCESS_ACCUMULATE);
    CHECK_ARG(pConfig->inputCfg.format == AUDIO_FORMAT_PCM_16_BIT);

    pContext->config = *pConfig;


 switch (pConfig->inputCfg.samplingRate) {
 case 8000:
 SampleRate = LVM_FS_8000;
 break;
 case 16000:
 SampleRate = LVM_FS_16000;
 break;
 case 22050:
 SampleRate = LVM_FS_22050;
 break;
 case 32000:
 SampleRate = LVM_FS_32000;
 break;
 case 44100:
 SampleRate = LVM_FS_44100;
 break;
 case 48000:
 SampleRate = LVM_FS_48000;
 break;
 default:
        ALOGV("\rReverb_setConfig invalid sampling rate %d", pConfig->inputCfg.samplingRate);
 return -EINVAL;
 }

 if (pContext->SampleRate != SampleRate) {

        LVREV_ControlParams_st    ActiveParams;
        LVREV_ReturnStatus_en     LvmStatus = LVREV_SUCCESS;


  
 LvmStatus = LVREV_GetControlParameters(pContext->hInstance,
 &ActiveParams);

        LVM_ERROR_CHECK(LvmStatus, "LVREV_GetControlParameters", "Reverb_setConfig")
 if(LvmStatus != LVREV_SUCCESS) return -EINVAL;

 ActiveParams.SampleRate = SampleRate;

 LvmStatus = LVREV_SetControlParameters(pContext->hInstance, &ActiveParams);

        LVM_ERROR_CHECK(LvmStatus, "LVREV_SetControlParameters", "Reverb_setConfig")
 if(LvmStatus != LVREV_SUCCESS) return -EINVAL;
        pContext->SampleRate = SampleRate;
 }else{
 }

 return 0;
}  
