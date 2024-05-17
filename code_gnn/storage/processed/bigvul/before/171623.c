static int a2dp_read_audio_config(struct a2dp_stream_common *common)
{
 char cmd = A2DP_CTRL_GET_AUDIO_CONFIG;
 uint32_t sample_rate;
 uint8_t channel_count;

 if (a2dp_command(common, A2DP_CTRL_GET_AUDIO_CONFIG) < 0)
 {
        ERROR("check a2dp ready failed");
 return -1;
 }

 if (a2dp_ctrl_receive(common, &sample_rate, 4) < 0)
 return -1;
 if (a2dp_ctrl_receive(common, &channel_count, 1) < 0)
 return -1;

    common->cfg.channel_flags = (channel_count == 1 ? AUDIO_CHANNEL_IN_MONO : AUDIO_CHANNEL_IN_STEREO);
    common->cfg.format = AUDIO_STREAM_DEFAULT_FORMAT;
    common->cfg.rate = sample_rate;

    INFO("got config %d %d", common->cfg.format, common->cfg.rate);

 return 0;
}
