void impeg2d_dec_seq_disp_ext(dec_state_t *ps_dec)
{
 stream_t *ps_stream;
    ps_stream = &ps_dec->s_bit_stream;

  

    impeg2d_bit_stream_get(ps_stream, 4);
    ps_dec->u1_video_format = impeg2d_bit_stream_get(ps_stream, 3);
    ps_dec->u1_colour_description = impeg2d_bit_stream_get(ps_stream, 1);
    ps_dec->u1_colour_primaries = 2;
    ps_dec->u1_transfer_characteristics = 2;
    ps_dec->u1_matrix_coefficients = 2;
 if(ps_dec->u1_colour_description)
 {
        ps_dec->u1_colour_primaries = impeg2d_bit_stream_get(ps_stream, 8);
        ps_dec->u1_transfer_characteristics = impeg2d_bit_stream_get(ps_stream, 8);
        ps_dec->u1_matrix_coefficients = impeg2d_bit_stream_get(ps_stream, 8);
 }

  
    ps_dec->u2_display_horizontal_size = impeg2d_bit_stream_get(ps_stream,14);;
    GET_MARKER_BIT(ps_dec,ps_stream);
    ps_dec->u2_display_vertical_size   = impeg2d_bit_stream_get(ps_stream,14);

    ps_dec->u1_seq_disp_extn_present = 1;
    impeg2d_next_start_code(ps_dec);
}
