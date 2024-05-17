int Downmix_Init(downmix_module_t *pDwmModule) {

    ALOGV("Downmix_Init module %p", pDwmModule);
 int ret = 0;

    memset(&pDwmModule->context, 0, sizeof(downmix_object_t));

    pDwmModule->config.inputCfg.accessMode = EFFECT_BUFFER_ACCESS_READ;
    pDwmModule->config.inputCfg.format = AUDIO_FORMAT_PCM_16_BIT;
    pDwmModule->config.inputCfg.channels = AUDIO_CHANNEL_OUT_7POINT1;
    pDwmModule->config.inputCfg.bufferProvider.getBuffer = NULL;
    pDwmModule->config.inputCfg.bufferProvider.releaseBuffer = NULL;
    pDwmModule->config.inputCfg.bufferProvider.cookie = NULL;
    pDwmModule->config.inputCfg.mask = EFFECT_CONFIG_ALL;

    pDwmModule->config.inputCfg.samplingRate = 44100;
    pDwmModule->config.outputCfg.samplingRate = pDwmModule->config.inputCfg.samplingRate;

    pDwmModule->config.outputCfg.accessMode = EFFECT_BUFFER_ACCESS_ACCUMULATE;
    pDwmModule->config.outputCfg.format = AUDIO_FORMAT_PCM_16_BIT;
    pDwmModule->config.outputCfg.channels = AUDIO_CHANNEL_OUT_STEREO;
    pDwmModule->config.outputCfg.bufferProvider.getBuffer = NULL;
    pDwmModule->config.outputCfg.bufferProvider.releaseBuffer = NULL;
    pDwmModule->config.outputCfg.bufferProvider.cookie = NULL;
    pDwmModule->config.outputCfg.mask = EFFECT_CONFIG_ALL;

    ret = Downmix_Configure(pDwmModule, &pDwmModule->config, true);
 if (ret != 0) {
        ALOGV("Downmix_Init error %d on module %p", ret, pDwmModule);
 } else {
        pDwmModule->context.state = DOWNMIX_STATE_INITIALIZED;
 }

 return ret;
}
