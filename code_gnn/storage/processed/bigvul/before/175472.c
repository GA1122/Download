static void in_update_aux_channels(struct stream_in *in,
 effect_handle_t effect)
{
 uint32_t aux_channels;
 channel_config_t channel_config;
 int status;

    aux_channels = in_get_aux_channels(in);

    channel_config.main_channels = in->main_channels;
    channel_config.aux_channels = aux_channels;
    status = in_reconfigure_channels(in,
                                     effect,
 &channel_config,
 (aux_channels != in->aux_channels));

 if (status != 0) {
        ALOGV("in_update_aux_channels(): in_reconfigure_channels error %d", status);
  
        aux_channels = 0;
        channel_config.aux_channels = 0;
        in_reconfigure_channels(in, effect, &channel_config, true);
 }
    ALOGV("%s: aux_channels=%d, in->aux_channels_changed=%d", __func__, aux_channels, in->aux_channels_changed);
 if (in->aux_channels != aux_channels) {
        in->aux_channels_changed = true;
        in->aux_channels = aux_channels;
        do_in_standby_l(in);
 }
}
