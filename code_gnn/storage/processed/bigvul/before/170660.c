void Session_GetReverseConfig(preproc_session_t *session, effect_config_t *config)
{
    memset(config, 0, sizeof(effect_config_t));
    config->inputCfg.samplingRate = config->outputCfg.samplingRate = session->samplingRate;
    config->inputCfg.format = config->outputCfg.format = AUDIO_FORMAT_PCM_16_BIT;
    config->inputCfg.channels = config->outputCfg.channels =
            audio_channel_in_mask_from_count(session->revChannelCount);
    config->inputCfg.mask = config->outputCfg.mask =
 (EFFECT_CONFIG_SMP_RATE | EFFECT_CONFIG_CHANNELS | EFFECT_CONFIG_FORMAT);
}
