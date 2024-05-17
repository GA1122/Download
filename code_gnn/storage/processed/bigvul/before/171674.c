static int stop_audio_datapath(struct a2dp_stream_common *common)
{
 int oldstate = common->state;

    INFO("state %d", common->state);

 if (common->ctrl_fd == AUDIO_SKT_DISCONNECTED)
 return -1;

  
    common->state = AUDIO_A2DP_STATE_STOPPING;

 if (a2dp_command(common, A2DP_CTRL_CMD_STOP) < 0)
 {
        ERROR("audiopath stop failed");
        common->state = oldstate;
 return -1;
 }

    common->state = AUDIO_A2DP_STATE_STOPPED;

  
    skt_disconnect(common->audio_fd);
    common->audio_fd = AUDIO_SKT_DISCONNECTED;

 return 0;
}
