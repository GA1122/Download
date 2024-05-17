static int suspend_audio_datapath(struct a2dp_stream_common *common, bool standby)
{
    INFO("state %d", common->state);

 if (common->ctrl_fd == AUDIO_SKT_DISCONNECTED)
 return -1;

 if (common->state == AUDIO_A2DP_STATE_STOPPING)
 return -1;

 if (a2dp_command(common, A2DP_CTRL_CMD_SUSPEND) < 0)
 return -1;

 if (standby)
        common->state = AUDIO_A2DP_STATE_STANDBY;
 else
        common->state = AUDIO_A2DP_STATE_SUSPENDED;

  
    skt_disconnect(common->audio_fd);

    common->audio_fd = AUDIO_SKT_DISCONNECTED;

 return 0;
}
