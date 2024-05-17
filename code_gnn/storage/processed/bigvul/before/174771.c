void impeg2d_dec_seq_disp_ext(dec_state_t *ps_dec)
{
 stream_t *ps_stream;
    ps_stream = &ps_dec->s_bit_stream;

  

    impeg2d_bit_stream_get(ps_stream,7);
 if (impeg2d_bit_stream_get_bit(ps_stream) == 1)
 {
        impeg2d_bit_stream_get(ps_stream,24);
 }

  
    ps_dec->u2_display_horizontal_size = impeg2d_bit_stream_get(ps_stream,14);;
    GET_MARKER_BIT(ps_dec,ps_stream);
    ps_dec->u2_display_vertical_size   = impeg2d_bit_stream_get(ps_stream,14);

    impeg2d_next_start_code(ps_dec);
}
