static uint32_t out_get_sample_rate(const struct audio_stream *stream)
{
 struct a2dp_stream_out *out = (struct a2dp_stream_out *)stream;

    DEBUG("rate %" PRIu32,out->common.cfg.rate);

 return out->common.cfg.rate;
}
