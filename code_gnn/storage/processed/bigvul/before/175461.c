static uint32_t in_get_sample_rate(const struct audio_stream *stream)
{
 struct stream_in *in = (struct stream_in *)stream;

 return in->requested_rate;
}
