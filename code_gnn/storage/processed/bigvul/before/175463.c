static int in_reconfigure_channels(struct stream_in *in,
 effect_handle_t effect,
 channel_config_t *channel_config,
 bool config_changed) {

 int status = 0;

    ALOGV("in_reconfigure_channels(): config_changed %d effect %p",
          config_changed, effect);

  
 if (config_changed) {
 int i;
        ALOGV("%s: config_changed (%d)", __func__, config_changed);
 for (i = 0; i < in->num_preprocessors; i++) {
 int cur_status = in_configure_effect_channels(in->preprocessors[i].effect_itfe,
                                                  channel_config);
            ALOGV("%s: in_configure_effect_channels i=(%d), [main_channel,aux_channel]=[%d|%d], status=%d",
                          __func__, i, channel_config->main_channels, channel_config->aux_channels, cur_status);
 if (cur_status != 0) {
                ALOGV("in_reconfigure_channels(): error %d configuring effect "
 "%d with channels: [%04x][%04x]",
                        cur_status,
                        i,
                        channel_config->main_channels,
                        channel_config->aux_channels);
                status = cur_status;
 }
 }
 } else if (effect != NULL && channel_config->aux_channels) {
  
        status = in_configure_effect_channels(effect, channel_config);
 }
 return status;
}
