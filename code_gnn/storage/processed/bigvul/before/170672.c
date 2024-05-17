int Visualizer_init(VisualizerContext *pContext)
{
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

    pContext->mCaptureSize = VISUALIZER_CAPTURE_SIZE_MAX;
    pContext->mScalingMode = VISUALIZER_SCALING_MODE_NORMALIZED;

    pContext->mChannelCount =
            audio_channel_count_from_out_mask(pContext->mConfig.inputCfg.channels);
    pContext->mMeasurementMode = MEASUREMENT_MODE_NONE;
    pContext->mMeasurementWindowSizeInBuffers = MEASUREMENT_WINDOW_MAX_SIZE_IN_BUFFERS;
    pContext->mMeasurementBufferIdx = 0;
 for (uint32_t i=0 ; i<pContext->mMeasurementWindowSizeInBuffers ; i++) {
        pContext->mPastMeasurements[i].mIsValid = false;
        pContext->mPastMeasurements[i].mPeakU16 = 0;
        pContext->mPastMeasurements[i].mRmsSquared = 0;
 }

 Visualizer_setConfig(pContext, &pContext->mConfig);

 return 0;
}
