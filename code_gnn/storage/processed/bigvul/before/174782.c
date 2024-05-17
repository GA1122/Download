IMPEG2D_ERROR_CODES_T impeg2d_process_video_header(dec_state_t *ps_dec)
{
 stream_t *ps_stream;
    ps_stream = &ps_dec->s_bit_stream;
    IMPEG2D_ERROR_CODES_T e_error;

    impeg2d_next_code(ps_dec, SEQUENCE_HEADER_CODE);
 if(ps_dec->s_bit_stream.u4_offset < ps_dec->s_bit_stream.u4_max_offset)
 {
        e_error = impeg2d_dec_seq_hdr(ps_dec);
 if ((IMPEG2D_ERROR_CODES_T)IVD_ERROR_NONE != e_error)
 {
 return e_error;
 }
 }
 else
 {
 return IMPEG2D_BITSTREAM_BUFF_EXCEEDED_ERR;
 }
 if (impeg2d_bit_stream_nxt(ps_stream,START_CODE_LEN) == EXTENSION_START_CODE)
 {
  
 if(ps_dec->s_bit_stream.u4_offset < ps_dec->s_bit_stream.u4_max_offset)
 {
            e_error = impeg2d_dec_seq_ext(ps_dec);
 if ((IMPEG2D_ERROR_CODES_T)IVD_ERROR_NONE != e_error)
 {
 return e_error;
 }
 }
 else
 {
 return IMPEG2D_BITSTREAM_BUFF_EXCEEDED_ERR;
 }
 if(ps_dec->s_bit_stream.u4_offset < ps_dec->s_bit_stream.u4_max_offset)
 {
            e_error = impeg2d_dec_seq_ext_data(ps_dec);
 if ((IMPEG2D_ERROR_CODES_T)IVD_ERROR_NONE != e_error)
 {
 return e_error;
 }
 }
 return impeg2d_init_video_state(ps_dec,MPEG_2_VIDEO);
 }
 else
 {
  
 if(ps_dec->s_bit_stream.u4_offset < ps_dec->s_bit_stream.u4_max_offset)
 {
            impeg2d_flush_ext_and_user_data(ps_dec);
 }
 return impeg2d_init_video_state(ps_dec,MPEG_1_VIDEO);
 }
}
