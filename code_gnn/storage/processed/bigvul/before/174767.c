void impeg2d_dec_pic_disp_ext(dec_state_t *ps_dec)
{
    WORD16 i2_number_of_frame_centre_offsets ;
 stream_t *ps_stream;

    ps_stream = &ps_dec->s_bit_stream;
    impeg2d_bit_stream_flush(ps_stream,4);

 if (ps_dec->u2_progressive_sequence)
 {
        i2_number_of_frame_centre_offsets = (ps_dec->u2_repeat_first_field) ?
 2 + ps_dec->u2_top_field_first : 1;
 }
 else
 {
        i2_number_of_frame_centre_offsets =
 (ps_dec->u2_picture_structure != FRAME_PICTURE) ?
 1 : 2 + ps_dec->u2_repeat_first_field;
 }
 while(i2_number_of_frame_centre_offsets--)
 {
  
        impeg2d_bit_stream_get(ps_stream,16);
        GET_MARKER_BIT(ps_dec,ps_stream);
  
        impeg2d_bit_stream_get(ps_stream,16);
        GET_MARKER_BIT(ps_dec,ps_stream);
 }
    impeg2d_next_start_code(ps_dec);
}
