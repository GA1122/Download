IMPEG2D_ERROR_CODES_T impeg2d_process_video_bit_stream(dec_state_t *ps_dec)
{
 stream_t *ps_stream;
    UWORD32 u4_next_bits, u4_start_code_found;
    IMPEG2D_ERROR_CODES_T e_error;

    ps_stream = &ps_dec->s_bit_stream;
    impeg2d_next_start_code(ps_dec);
  
    u4_start_code_found = 0;

 if(ps_dec->u2_is_mpeg2)
 {
  
 while((u4_start_code_found == 0) && (ps_dec->s_bit_stream.u4_offset < ps_dec->s_bit_stream.u4_max_offset))
 {
            u4_next_bits = impeg2d_bit_stream_nxt(ps_stream,START_CODE_LEN);

 if(u4_next_bits == SEQUENCE_HEADER_CODE)
 {
 if(ps_dec->s_bit_stream.u4_offset < ps_dec->s_bit_stream.u4_max_offset)
 {
                    e_error = impeg2d_dec_seq_hdr(ps_dec);
 if ((IMPEG2D_ERROR_CODES_T)IVD_ERROR_NONE != e_error)
 {
 return e_error;
 }

                    u4_start_code_found = 0;

 }
 else
 {
 return IMPEG2D_BITSTREAM_BUFF_EXCEEDED_ERR;
 }


 if(ps_dec->s_bit_stream.u4_offset < ps_dec->s_bit_stream.u4_max_offset)
 {
                    IMPEG2D_ERROR_CODES_T e_error;
                    e_error = impeg2d_dec_seq_ext(ps_dec);
 if ((IMPEG2D_ERROR_CODES_T)IVD_ERROR_NONE != e_error)
 {
 return e_error;
 }
                    u4_start_code_found = 0;

 }
 else
 {
 return IMPEG2D_BITSTREAM_BUFF_EXCEEDED_ERR;
 }
 }
 else if((u4_next_bits == USER_DATA_START_CODE) || (u4_next_bits == EXTENSION_START_CODE))
 {
 if(ps_dec->s_bit_stream.u4_offset < ps_dec->s_bit_stream.u4_max_offset)
 {
                    impeg2d_dec_seq_ext_data(ps_dec);
                    u4_start_code_found = 0;

 }

 }
 else if((ps_dec->s_bit_stream.u4_offset < ps_dec->s_bit_stream.u4_max_offset)
 && (u4_next_bits == GOP_START_CODE))
 {
                impeg2d_dec_grp_of_pic_hdr(ps_dec);
                impeg2d_dec_user_data(ps_dec);
                u4_start_code_found = 0;

 }
 else if((ps_dec->s_bit_stream.u4_offset < ps_dec->s_bit_stream.u4_max_offset)
 && (u4_next_bits == PICTURE_START_CODE))
 {
                ps_dec->i4_pic_count++;

                e_error = impeg2d_dec_pic_hdr(ps_dec);
 if ((IMPEG2D_ERROR_CODES_T)IVD_ERROR_NONE != e_error)
 {
 return e_error;
 }
                e_error = impeg2d_dec_pic_coding_ext(ps_dec);
 if ((IMPEG2D_ERROR_CODES_T)IVD_ERROR_NONE != e_error)
 {
 return e_error;
 }
                e_error = impeg2d_dec_pic_ext_data(ps_dec);
 if ((IMPEG2D_ERROR_CODES_T)IVD_ERROR_NONE != e_error)
 {
 return e_error;
 }
                e_error = impeg2d_pre_pic_dec_proc(ps_dec);
 if ((IMPEG2D_ERROR_CODES_T) IVD_ERROR_NONE != e_error)
 {
 return e_error;
 }
                impeg2d_dec_pic_data(ps_dec);
                impeg2d_post_pic_dec_proc(ps_dec);
                u4_start_code_found = 1;
 }
 else

 {
                FLUSH_BITS(ps_dec->s_bit_stream.u4_offset, ps_dec->s_bit_stream.u4_buf, ps_dec->s_bit_stream.u4_buf_nxt, 8, ps_dec->s_bit_stream.pu4_buf_aligned);

 }
 if(u4_start_code_found == 0)
 {
                impeg2d_next_start_code(ps_dec);
  
                ps_dec->i4_bytes_consumed = (ps_dec->s_bit_stream.u4_offset + 7) >> 3;
                ps_dec->i4_bytes_consumed -= ((size_t)ps_dec->s_bit_stream.pv_bs_buf & 3);
 }
 }
 if((u4_start_code_found == 0) && (ps_dec->s_bit_stream.u4_offset > ps_dec->s_bit_stream.u4_max_offset))
 {
 return IMPEG2D_FRM_HDR_START_CODE_NOT_FOUND;
 }

 }
  
 else
 {
 while((u4_start_code_found == 0) && (ps_dec->s_bit_stream.u4_offset < ps_dec->s_bit_stream.u4_max_offset))
 {
            u4_next_bits = impeg2d_bit_stream_nxt(ps_stream,START_CODE_LEN);

 if(impeg2d_bit_stream_nxt(ps_stream,START_CODE_LEN) == SEQUENCE_HEADER_CODE)
 {
 if(ps_dec->s_bit_stream.u4_offset < ps_dec->s_bit_stream.u4_max_offset)
 {
                    e_error = impeg2d_dec_seq_hdr(ps_dec);
 if ((IMPEG2D_ERROR_CODES_T)IVD_ERROR_NONE != e_error)
 {
 return e_error;
 }

                    u4_start_code_found = 0;
 }
 else
 {
 return IMPEG2D_BITSTREAM_BUFF_EXCEEDED_ERR;
 }
 }
 else if((ps_dec->s_bit_stream.u4_offset < ps_dec->s_bit_stream.u4_max_offset) && (u4_next_bits == EXTENSION_START_CODE || u4_next_bits == USER_DATA_START_CODE))
 {
                impeg2d_flush_ext_and_user_data(ps_dec);
                u4_start_code_found = 0;
 }


 else if ((impeg2d_bit_stream_nxt(ps_stream,START_CODE_LEN) == GOP_START_CODE)
 && (ps_dec->s_bit_stream.u4_offset < ps_dec->s_bit_stream.u4_max_offset))
 {
                impeg2d_dec_grp_of_pic_hdr(ps_dec);
                impeg2d_flush_ext_and_user_data(ps_dec);
                u4_start_code_found = 0;
 }
 else if ((impeg2d_bit_stream_nxt(ps_stream,START_CODE_LEN) == PICTURE_START_CODE)
 && (ps_dec->s_bit_stream.u4_offset < ps_dec->s_bit_stream.u4_max_offset))
 {
                ps_dec->i4_pic_count++;

                e_error = impeg2d_dec_pic_hdr(ps_dec);
 if ((IMPEG2D_ERROR_CODES_T)IVD_ERROR_NONE != e_error)
 {
 return e_error;
 }
                impeg2d_flush_ext_and_user_data(ps_dec);
                impeg2d_pre_pic_dec_proc(ps_dec);
                impeg2d_dec_pic_data(ps_dec);
                impeg2d_post_pic_dec_proc(ps_dec);
                u4_start_code_found = 1;
 }
 else
 {
                FLUSH_BITS(ps_dec->s_bit_stream.u4_offset, ps_dec->s_bit_stream.u4_buf, ps_dec->s_bit_stream.u4_buf_nxt, 8, ps_dec->s_bit_stream.pu4_buf_aligned);
 }
            impeg2d_next_start_code(ps_dec);
 if (0 == u4_start_code_found)
 {
  
                ps_dec->i4_bytes_consumed = (ps_dec->s_bit_stream.u4_offset + 7) >> 3;
                ps_dec->i4_bytes_consumed -= ((size_t)ps_dec->s_bit_stream.pv_bs_buf & 3);
 }
 }
 if((u4_start_code_found == 0) && (ps_dec->s_bit_stream.u4_offset > ps_dec->s_bit_stream.u4_max_offset))
 {
 return IMPEG2D_FRM_HDR_START_CODE_NOT_FOUND;
 }
 }

 return (IMPEG2D_ERROR_CODES_T)IVD_ERROR_NONE;
}
