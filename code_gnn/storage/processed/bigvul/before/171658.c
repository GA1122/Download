static int out_dump(const struct audio_stream *stream, int fd)
{
    UNUSED(fd);
 struct a2dp_stream_out *out = (struct a2dp_stream_out *)stream;
    FNLOG();
 return 0;
}
