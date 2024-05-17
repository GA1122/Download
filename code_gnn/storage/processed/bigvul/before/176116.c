IMPEG2D_ERROR_CODES_T impeg2d_dec_seq_ext(dec_state_t *ps_dec)
{
 stream_t *ps_stream;
    UWORD16 horizontal_value;
    UWORD16 vertical_value;

    ps_stream = &ps_dec->s_bit_stream;

 if (impeg2d_bit_stream_nxt(ps_stream,START_CODE_LEN) != EXTENSION_START_CODE)
 {
        impeg2d_bit_stream_flush(ps_stream,START_CODE_LEN);
 return IMPEG2D_FRM_HDR_START_CODE_NOT_FOUND;

 }
  
    impeg2d_bit_stream_flush(ps_stream,START_CODE_LEN);

  
    impeg2d_bit_stream_flush(ps_stream,4);

  
  
  
 {
        UWORD32   u4_esc_bit, u4_profile, u4_level;

  
  
  
  
  

        u4_esc_bit   = impeg2d_bit_stream_get_bit(ps_stream);
        u4_profile   = impeg2d_bit_stream_get(ps_stream,3);
        u4_level     = impeg2d_bit_stream_get(ps_stream,4);
        UNUSED(u4_profile);
        UNUSED(u4_level);
  
 if (1 == u4_esc_bit)
 {
 return IMPEG2D_PROF_LEVEL_NOT_SUPPORTED;
 }
 }

    ps_dec->u2_progressive_sequence = impeg2d_bit_stream_get_bit(ps_stream);

  
 if(impeg2d_bit_stream_get(ps_stream,2) != 0x1)
 return IMPEG2D_CHROMA_FMT_NOT_SUP;

  
  
  


  
    horizontal_value               = (impeg2d_bit_stream_get(ps_stream,2) << 12);

  
    vertical_value                 = (impeg2d_bit_stream_get(ps_stream,2) << 12);

  
  
 if(ps_dec->u2_create_max_height < (ps_dec->u2_vertical_size + vertical_value))
 {
 return (IMPEG2D_ERROR_CODES_T) IVD_STREAM_WIDTH_HEIGHT_NOT_SUPPORTED;
 }

 if(ps_dec->u2_create_max_width < (ps_dec->u2_horizontal_size + horizontal_value))
 {
 return (IMPEG2D_ERROR_CODES_T) IVD_STREAM_WIDTH_HEIGHT_NOT_SUPPORTED;
 }
    ps_dec->u2_vertical_size       += vertical_value;
    ps_dec->u2_horizontal_size     += horizontal_value;

  
  
  
  
  
  
  
    impeg2d_bit_stream_flush(ps_stream,12);
    GET_MARKER_BIT(ps_dec,ps_stream);
    impeg2d_bit_stream_flush(ps_stream,9);
  
  
  
  
    ps_dec->u2_frame_rate_extension_n = impeg2d_bit_stream_get(ps_stream,2);
    ps_dec->u2_frame_rate_extension_d = impeg2d_bit_stream_get(ps_stream,5);

 return (IMPEG2D_ERROR_CODES_T)IVD_ERROR_NONE;
}
