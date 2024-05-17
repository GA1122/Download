static int out_standby(struct audio_stream *stream)
{
 struct a2dp_stream_out *out = (struct a2dp_stream_out *)stream;
 int retVal = 0;

    FNLOG();

    pthread_mutex_lock(&out->common.lock);
 if (out->common.state != AUDIO_A2DP_STATE_SUSPENDED)
        retVal = suspend_audio_datapath(&out->common, true);
    out->frames_rendered = 0;  
    pthread_mutex_unlock (&out->common.lock);

 return retVal;
}
