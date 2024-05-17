static audio_format_t out_get_format(const struct audio_stream *stream)
{
 struct stream_out *out = (struct stream_out *)stream;

 return out->format;
}
