static int out_set_sample_rate(struct audio_stream *stream, uint32_t rate)
{
 struct a2dp_stream_out *out = (struct a2dp_stream_out *)stream;

    DEBUG("out_set_sample_rate : %" PRIu32, rate);

 if (rate != AUDIO_STREAM_DEFAULT_RATE)
 {
        ERROR("only rate %d supported", AUDIO_STREAM_DEFAULT_RATE);
 return -1;
 }

    out->common.cfg.rate = rate;

 return 0;
}
