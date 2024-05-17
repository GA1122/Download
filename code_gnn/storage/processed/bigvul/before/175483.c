static uint32_t out_get_latency(const struct audio_stream_out *stream)
{
 struct stream_out *out = (struct stream_out *)stream;

 return (out->config.period_count * out->config.period_size * 1000) /
 (out->config.rate);
}
