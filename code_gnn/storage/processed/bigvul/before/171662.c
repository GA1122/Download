static char * out_get_parameters(const struct audio_stream *stream, const char *keys)
{
    UNUSED(keys);
 struct a2dp_stream_out *out = (struct a2dp_stream_out *)stream;

    FNLOG();

  

 return strdup("");
}
