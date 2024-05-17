static int out_get_render_position(const struct audio_stream_out *stream,
 uint32_t *dsp_frames)
{
 (void)stream;
 *dsp_frames = 0;
 return -EINVAL;
}
