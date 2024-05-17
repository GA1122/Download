void impeg2d_dec_pic_coding_ext(dec_state_t *ps_dec)
{
 stream_t *ps_stream;

    ps_stream = &ps_dec->s_bit_stream;
    impeg2d_bit_stream_flush(ps_stream,START_CODE_LEN);
  
    impeg2d_bit_stream_get(ps_stream,4);

    ps_dec->au2_f_code[0][0] = impeg2d_bit_stream_get(ps_stream,4);
    ps_dec->au2_f_code[0][1] = impeg2d_bit_stream_get(ps_stream,4);
    ps_dec->au2_f_code[1][0] = impeg2d_bit_stream_get(ps_stream,4);
    ps_dec->au2_f_code[1][1] = impeg2d_bit_stream_get(ps_stream,4);
    ps_dec->u2_intra_dc_precision        = impeg2d_bit_stream_get(ps_stream,2);
    ps_dec->u2_picture_structure            = impeg2d_bit_stream_get(ps_stream,2);
    ps_dec->u2_top_field_first              = impeg2d_bit_stream_get_bit(ps_stream);
    ps_dec->u2_frame_pred_frame_dct         = impeg2d_bit_stream_get_bit(ps_stream);
    ps_dec->u2_concealment_motion_vectors   = impeg2d_bit_stream_get_bit(ps_stream);
    ps_dec->u2_q_scale_type                 = impeg2d_bit_stream_get_bit(ps_stream);
    ps_dec->u2_intra_vlc_format             = impeg2d_bit_stream_get_bit(ps_stream);
    ps_dec->u2_alternate_scan               = impeg2d_bit_stream_get_bit(ps_stream);
    ps_dec->u2_repeat_first_field           = impeg2d_bit_stream_get_bit(ps_stream);
  
    impeg2d_bit_stream_get_bit(ps_stream);

    ps_dec->u2_progressive_frame            = impeg2d_bit_stream_get_bit(ps_stream);
 if (impeg2d_bit_stream_get_bit(ps_stream))
 {
  
        impeg2d_bit_stream_flush(ps_stream,20);
 }
    impeg2d_next_start_code(ps_dec);


 if(VERTICAL_SCAN == ps_dec->u2_alternate_scan)
 {
        ps_dec->pu1_inv_scan_matrix = (UWORD8 *)gau1_impeg2_inv_scan_vertical;
 }
 else
 {
        ps_dec->pu1_inv_scan_matrix = (UWORD8 *)gau1_impeg2_inv_scan_zig_zag;
 }
}