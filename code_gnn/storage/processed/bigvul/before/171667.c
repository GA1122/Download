static int out_set_format(struct audio_stream *stream, audio_format_t format)
{
    UNUSED(format);
 struct a2dp_stream_out *out = (struct a2dp_stream_out *)stream;
    DEBUG("setting format not yet supported (0x%x)", format);
 return -ENOSYS;
}
