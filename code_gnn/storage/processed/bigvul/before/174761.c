void impeg2d_dec_cam_param_ext(dec_state_t *ps_dec)
{

    UWORD32 u4_bits_to_flush;

    u4_bits_to_flush = CAMERA_PARAMETER_EXTENSION_LEN;

 while(u4_bits_to_flush >= 32 )
 {
        impeg2d_bit_stream_flush(&ps_dec->s_bit_stream,32);
        u4_bits_to_flush = u4_bits_to_flush - 32;
 }

 if(u4_bits_to_flush > 0)
 {
        impeg2d_bit_stream_flush(&ps_dec->s_bit_stream,u4_bits_to_flush);
 }

  impeg2d_next_start_code(ps_dec);
}
