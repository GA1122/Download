static void adev_close_input_stream(struct audio_hw_device *dev,
 struct audio_stream_in *stream)
{
 struct a2dp_audio_device *a2dp_dev = (struct a2dp_audio_device *)dev;
 struct a2dp_stream_in* in = (struct a2dp_stream_in *)stream;
 a2dp_state_t state = in->common.state;

    INFO("closing input (state %d)", state);

 if ((state == AUDIO_A2DP_STATE_STARTED) || (state == AUDIO_A2DP_STATE_STOPPING))
        stop_audio_datapath(&in->common);

    skt_disconnect(in->common.ctrl_fd);
    free(stream);
    a2dp_dev->input = NULL;

    DEBUG("done");
}
