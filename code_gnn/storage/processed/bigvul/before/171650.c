static uint32_t in_get_sample_rate(const struct audio_stream *stream)
{
 struct a2dp_stream_in *in = (struct a2dp_stream_in *)stream;

    FNLOG();
 return in->common.cfg.rate;
}
