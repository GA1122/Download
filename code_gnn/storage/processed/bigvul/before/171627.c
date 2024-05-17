static void adev_close_output_stream(struct audio_hw_device *dev,
 struct audio_stream_out *stream)
{
 struct a2dp_audio_device *a2dp_dev = (struct a2dp_audio_device *)dev;
 struct a2dp_stream_out *out = (struct a2dp_stream_out *)stream;

    INFO("closing output (state %d)", out->common.state);

    pthread_mutex_lock(&out->common.lock);
 if ((out->common.state == AUDIO_A2DP_STATE_STARTED) || (out->common.state == AUDIO_A2DP_STATE_STOPPING))
        stop_audio_datapath(&out->common);

    skt_disconnect(out->common.ctrl_fd);
    free(stream);
    a2dp_dev->output = NULL;
    pthread_mutex_unlock(&out->common.lock);

    DEBUG("done");
}
