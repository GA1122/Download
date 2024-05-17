static audio_format_t out_get_format(const struct audio_stream *stream)
{
 struct a2dp_stream_out *out = (struct a2dp_stream_out *)stream;
    DEBUG("format 0x%x", out->common.cfg.format);
 return out->common.cfg.format;
}
