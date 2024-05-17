void impeg2d_dec_itu_t_ext(dec_state_t *ps_dec)
{
  impeg2d_bit_stream_flush(&ps_dec->s_bit_stream,EXT_ID_LEN);
  impeg2d_next_start_code(ps_dec);
}
