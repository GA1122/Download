int Effect_setConfig(EffectContext *pContext, effect_config_t *pConfig){
    LVM_Fs_en   SampleRate;

    CHECK_ARG(pContext != NULL);
    CHECK_ARG(pConfig != NULL);

    CHECK_ARG(pConfig->inputCfg.samplingRate == pConfig->outputCfg.samplingRate);
    CHECK_ARG(pConfig->inputCfg.channels == pConfig->outputCfg.channels);
    CHECK_ARG(pConfig->inputCfg.format == pConfig->outputCfg.format);
    CHECK_ARG(pConfig->inputCfg.channels == AUDIO_CHANNEL_OUT_STEREO);
    CHECK_ARG(pConfig->outputCfg.accessMode == EFFECT_BUFFER_ACCESS_WRITE
 || pConfig->outputCfg.accessMode == EFFECT_BUFFER_ACCESS_ACCUMULATE);
    CHECK_ARG(pConfig->inputCfg.format == AUDIO_FORMAT_PCM_16_BIT);

    pContext->config = *pConfig;

 switch (pConfig->inputCfg.samplingRate) {
 case 8000:
 SampleRate = LVM_FS_8000;
        pContext->pBundledContext->SamplesPerSecond = 8000*2;  
 break;
 case 16000:
 SampleRate = LVM_FS_16000;
        pContext->pBundledContext->SamplesPerSecond = 16000*2;  
 break;
 case 22050:
 SampleRate = LVM_FS_22050;
        pContext->pBundledContext->SamplesPerSecond = 22050*2;  
 break;
 case 32000:
 SampleRate = LVM_FS_32000;
        pContext->pBundledContext->SamplesPerSecond = 32000*2;  
 break;
 case 44100:
 SampleRate = LVM_FS_44100;
        pContext->pBundledContext->SamplesPerSecond = 44100*2;  
 break;
 case 48000:
 SampleRate = LVM_FS_48000;
        pContext->pBundledContext->SamplesPerSecond = 48000*2;  
 break;
 default:
        ALOGV("\tEffect_setConfig invalid sampling rate %d", pConfig->inputCfg.samplingRate);
 return -EINVAL;
 }

 if(pContext->pBundledContext->SampleRate != SampleRate){

 LVM_ControlParams_t ActiveParams;
        LVM_ReturnStatus_en     LvmStatus = LVM_SUCCESS;

        ALOGV("\tEffect_setConfig change sampling rate to %d", SampleRate);

  
 LvmStatus = LVM_GetControlParameters(pContext->pBundledContext->hInstance,
 &ActiveParams);

        LVM_ERROR_CHECK(LvmStatus, "LVM_GetControlParameters", "Effect_setConfig")
 if(LvmStatus != LVM_SUCCESS) return -EINVAL;

 ActiveParams.SampleRate = SampleRate;

 LvmStatus = LVM_SetControlParameters(pContext->pBundledContext->hInstance, &ActiveParams);

        LVM_ERROR_CHECK(LvmStatus, "LVM_SetControlParameters", "Effect_setConfig")
        ALOGV("\tEffect_setConfig Succesfully called LVM_SetControlParameters\n");
        pContext->pBundledContext->SampleRate = SampleRate;

 }else{
 }

 return 0;
}  
