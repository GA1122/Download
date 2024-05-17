static int start_audio_datapath(struct a2dp_stream_common *common)
{
    INFO("state %d", common->state);

 if (common->ctrl_fd == AUDIO_SKT_DISCONNECTED) {
        INFO("%s AUDIO_SKT_DISCONNECTED", __func__);
 return -1;
 }

 int oldstate = common->state;
    common->state = AUDIO_A2DP_STATE_STARTING;

 int a2dp_status = a2dp_command(common, A2DP_CTRL_CMD_START);
 if (a2dp_status < 0)
 {
        ERROR("%s Audiopath start failed (status %d)", __func__, a2dp_status);

        common->state = oldstate;
 return -1;
 }
 else if (a2dp_status == A2DP_CTRL_ACK_INCALL_FAILURE)
 {
        ERROR("%s Audiopath start failed - in call, move to suspended", __func__);
        common->state = oldstate;
 return -1;
 }

  
 if (common->audio_fd == AUDIO_SKT_DISCONNECTED)
 {
        common->audio_fd = skt_connect(A2DP_DATA_PATH, common->buffer_sz);
 if (common->audio_fd < 0)
 {
            common->state = oldstate;
 return -1;
 }

        common->state = AUDIO_A2DP_STATE_STARTED;
 }

 return 0;
}
