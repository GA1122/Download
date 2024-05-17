static uint32_t out_get_sample_rate(const struct audio_stream *stream)
{
 struct stream_out *out = (struct stream_out *)stream;

 return out->sample_rate;
}
