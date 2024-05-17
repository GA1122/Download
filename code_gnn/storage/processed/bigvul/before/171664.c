static int out_get_render_position(const struct audio_stream_out *stream,
 uint32_t *dsp_frames)
{
 struct a2dp_stream_out *out = (struct a2dp_stream_out *)stream;

    FNLOG();
 if (stream == NULL || dsp_frames == NULL)
 return -EINVAL;

    pthread_mutex_lock(&out->common.lock);
 uint64_t latency_frames = (uint64_t)out_get_latency(stream) * out->common.cfg.rate / 1000;
 if (out->frames_rendered >= latency_frames) {
 *dsp_frames = (uint32_t)(out->frames_rendered - latency_frames);
 } else {
 *dsp_frames = 0;
 }
    pthread_mutex_unlock(&out->common.lock);
 return 0;
}
