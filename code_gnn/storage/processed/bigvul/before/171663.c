static int out_get_presentation_position(const struct audio_stream_out *stream,
 uint64_t *frames, struct timespec *timestamp)
{
 struct a2dp_stream_out *out = (struct a2dp_stream_out *)stream;

    FNLOG();
 if (stream == NULL || frames == NULL || timestamp == NULL)
 return -EINVAL;

 int ret = -EWOULDBLOCK;
    pthread_mutex_lock(&out->common.lock);
 uint64_t latency_frames = (uint64_t)out_get_latency(stream) * out->common.cfg.rate / 1000;
 if (out->frames_presented >= latency_frames) {
 *frames = out->frames_presented - latency_frames;
        clock_gettime(CLOCK_MONOTONIC, timestamp);  
        ret = 0;
 }
    pthread_mutex_unlock(&out->common.lock);
 return ret;
}
