static int session_set_config(struct session_s *session, effect_config_t *config)
{
 int status;

 if (config->inputCfg.samplingRate != config->outputCfg.samplingRate ||
            config->inputCfg.format != config->outputCfg.format ||
            config->inputCfg.format != AUDIO_FORMAT_PCM_16_BIT)
 return -EINVAL;

    ALOGV("session_set_config() sampling rate %d channels %08x",
         config->inputCfg.samplingRate, config->inputCfg.channels);

 if (session->enabled_msk) {
 if (session->config.inputCfg.samplingRate != config->inputCfg.samplingRate ||
                session->config.inputCfg.channels != config->inputCfg.channels ||
                session->config.outputCfg.channels != config->outputCfg.channels)
 return -ENOSYS;
 else
 return 0;
 }

    memcpy(&session->config, config, sizeof(effect_config_t));

    session->state = SESSION_STATE_CONFIG;
 return 0;
}
