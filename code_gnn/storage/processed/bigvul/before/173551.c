static void session_get_config(struct session_s *session, effect_config_t *config)
{
    memcpy(config, &session->config, sizeof(effect_config_t));

    config->inputCfg.mask = config->outputCfg.mask =
 (EFFECT_CONFIG_SMP_RATE | EFFECT_CONFIG_CHANNELS | EFFECT_CONFIG_FORMAT);
}
