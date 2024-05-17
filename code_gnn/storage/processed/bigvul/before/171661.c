static uint32_t out_get_latency(const struct audio_stream_out *stream)
{
 int latency_us;

 struct a2dp_stream_out *out = (struct a2dp_stream_out *)stream;

    FNLOG();

    latency_us = ((out->common.buffer_sz * 1000 ) /
                    audio_stream_out_frame_size(&out->stream) /
                    out->common.cfg.rate) * 1000;


 return (latency_us / 1000) + 200;
}
