int LE_init(LoudnessEnhancerContext *pContext)
{
    ALOGV("LE_init(%p)", pContext);

    pContext->mConfig.inputCfg.accessMode = EFFECT_BUFFER_ACCESS_READ;
    pContext->mConfig.inputCfg.channels = AUDIO_CHANNEL_OUT_STEREO;
    pContext->mConfig.inputCfg.format = AUDIO_FORMAT_PCM_16_BIT;
    pContext->mConfig.inputCfg.samplingRate = 44100;
    pContext->mConfig.inputCfg.bufferProvider.getBuffer = NULL;
    pContext->mConfig.inputCfg.bufferProvider.releaseBuffer = NULL;
    pContext->mConfig.inputCfg.bufferProvider.cookie = NULL;
    pContext->mConfig.inputCfg.mask = EFFECT_CONFIG_ALL;
    pContext->mConfig.outputCfg.accessMode = EFFECT_BUFFER_ACCESS_ACCUMULATE;
    pContext->mConfig.outputCfg.channels = AUDIO_CHANNEL_OUT_STEREO;
    pContext->mConfig.outputCfg.format = AUDIO_FORMAT_PCM_16_BIT;
    pContext->mConfig.outputCfg.samplingRate = 44100;
    pContext->mConfig.outputCfg.bufferProvider.getBuffer = NULL;
    pContext->mConfig.outputCfg.bufferProvider.releaseBuffer = NULL;
    pContext->mConfig.outputCfg.bufferProvider.cookie = NULL;
    pContext->mConfig.outputCfg.mask = EFFECT_CONFIG_ALL;

    pContext->mTargetGainmB = LOUDNESS_ENHANCER_DEFAULT_TARGET_GAIN_MB;
 float targetAmp = pow(10, pContext->mTargetGainmB/2000.0f);  
    ALOGV("LE_init(): Target gain=%dmB <=> factor=%.2fX", pContext->mTargetGainmB, targetAmp);

 if (pContext->mCompressor == NULL) {
        pContext->mCompressor = new le_fx::AdaptiveDynamicRangeCompression();
        pContext->mCompressor->Initialize(targetAmp, pContext->mConfig.inputCfg.samplingRate);
 }

    LE_setConfig(pContext, &pContext->mConfig);

 return 0;
}
