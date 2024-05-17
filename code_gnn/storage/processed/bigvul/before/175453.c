static int in_configure_effect_channels(effect_handle_t effect,
 channel_config_t *channel_config)
{
 int status = 0;
 int fct_status;
 int32_t cmd_status;
 uint32_t reply_size;
 effect_config_t config;
 uint32_t cmd[(sizeof(uint32_t) + sizeof(channel_config_t) - 1) / sizeof(uint32_t) + 1];

    ALOGV("in_configure_effect_channels(): configure effect with channels: [%04x][%04x]",
            channel_config->main_channels,
            channel_config->aux_channels);

    config.inputCfg.mask = EFFECT_CONFIG_CHANNELS;
    config.outputCfg.mask = EFFECT_CONFIG_CHANNELS;
    reply_size = sizeof(effect_config_t);
    fct_status = (*effect)->command(effect,
                                EFFECT_CMD_GET_CONFIG,
 0,
                                NULL,
 &reply_size,
 &config);
 if (fct_status != 0) {
        ALOGE("in_configure_effect_channels(): EFFECT_CMD_GET_CONFIG failed");
 return fct_status;
 }

    config.inputCfg.channels = channel_config->aux_channels;
    config.outputCfg.channels = config.inputCfg.channels;
    reply_size = sizeof(uint32_t);
    fct_status = (*effect)->command(effect,
                                    EFFECT_CMD_SET_CONFIG,
 sizeof(effect_config_t),
 &config,
 &reply_size,
 &cmd_status);
    status = get_command_status(status, fct_status, cmd_status);
 if (status != 0) {
        ALOGE("in_configure_effect_channels(): EFFECT_CMD_SET_CONFIG failed");
 return status;
 }

  
    reply_size = sizeof(uint32_t);
    fct_status = (*effect)->command(effect,
                                  EFFECT_CMD_ENABLE,
 0,
                                  NULL,
 &reply_size,
 &cmd_status);
    status = get_command_status(status, fct_status, cmd_status);
 if (status != 0) {
        ALOGE("in_configure_effect_channels(): EFFECT_CMD_ENABLE failed");
 return status;
 }

 return status;
}
