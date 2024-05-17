static audio_format_t in_get_format(const struct audio_stream *stream)
{
 (void)stream;
 return AUDIO_FORMAT_PCM_16_BIT;
}
