void impeg2d_dec_frm(void *pv_dec,impeg2d_video_decode_ip_t *ps_ip,
 impeg2d_video_decode_op_t *ps_op)
{


 stream_t *ps_stream;
    UWORD32 u4_size = ps_ip->s_ivd_video_decode_ip_t.u4_num_Bytes;

 dec_state_t *ps_dec;

    ps_dec = (dec_state_t *)pv_dec;
    ps_op->s_ivd_video_decode_op_t.u4_error_code = 0;

    IMPEG2D_FRM_NUM_SET();

    ps_dec->pu1_inp_bits_buf = ps_ip->s_ivd_video_decode_ip_t.pv_stream_buffer;
    ps_dec->u4_num_inp_bytes = u4_size;
    ps_stream  = &ps_dec->s_bit_stream;


    impeg2d_bit_stream_init(ps_stream,ps_ip->s_ivd_video_decode_ip_t.pv_stream_buffer,u4_size);

    

    ps_dec->u4_xdmBufID     = ps_ip->s_ivd_video_decode_ip_t.u4_ts;

 {
        IMPEG2D_ERROR_CODES_T e_error;
  
        e_error = impeg2d_process_video_bit_stream(ps_dec);
 if ((IMPEG2D_ERROR_CODES_T)IVD_ERROR_NONE != e_error)
 {
            ps_op->s_ivd_video_decode_op_t.u4_error_code    = e_error;

 if ((IMPEG2D_ERROR_CODES_T) IVD_RES_CHANGED == e_error)
 {
                ps_op->s_ivd_video_decode_op_t.u4_num_bytes_consumed = 0;
                ps_dec->u2_header_done = 0;
 }
 else if (IMPEG2D_UNSUPPORTED_DIMENSIONS == e_error)
 {
                ps_op->s_ivd_video_decode_op_t.u4_num_bytes_consumed = 0;
                ps_dec->u2_header_done = 0;

                ps_op->s_ivd_video_decode_op_t.u4_pic_ht = ps_dec->u2_reinit_max_height;
                ps_op->s_ivd_video_decode_op_t.u4_pic_wd = ps_dec->u2_reinit_max_width;
 }
 else
 {
 if(ps_dec->i4_num_cores > 1)
                    ps_op->s_ivd_video_decode_op_t.u4_num_bytes_consumed = ps_dec->i4_bytes_consumed;
 else
 {
                    ps_op->s_ivd_video_decode_op_t.u4_num_bytes_consumed = (ps_dec->s_bit_stream.u4_offset + 7) >> 3;
                    ps_op->s_ivd_video_decode_op_t.u4_num_bytes_consumed -= ((size_t)ps_dec->s_bit_stream.pv_bs_buf & 3);
 }

 if(ps_op->s_ivd_video_decode_op_t.u4_num_bytes_consumed
 > ps_ip->s_ivd_video_decode_ip_t.u4_num_Bytes)
 {
                    ps_op->s_ivd_video_decode_op_t.u4_num_bytes_consumed =
                                    ps_ip->s_ivd_video_decode_ip_t.u4_num_Bytes;
 }

                impeg2d_next_start_code(ps_dec);
 }

 if(ps_op->s_ivd_video_decode_op_t.u4_error_code == 0)
 {
                ps_op->s_ivd_video_decode_op_t.u4_error_code = e_error;
 }

 return;
 }
 }
  
  
  
    ps_op->s_ivd_video_decode_op_t.u4_error_code  = IV_SUCCESS;

 if(ps_dec->i4_num_cores > 1)
        ps_op->s_ivd_video_decode_op_t.u4_num_bytes_consumed = ps_dec->i4_bytes_consumed;
 else
 {
        ps_op->s_ivd_video_decode_op_t.u4_num_bytes_consumed = (ps_dec->s_bit_stream.u4_offset + 7) >> 3;
        ps_op->s_ivd_video_decode_op_t.u4_num_bytes_consumed -= ((size_t)ps_dec->s_bit_stream.pv_bs_buf & 3);
 }
 if(ps_op->s_ivd_video_decode_op_t.u4_num_bytes_consumed > ps_ip->s_ivd_video_decode_ip_t.u4_num_Bytes)
 {
        ps_op->s_ivd_video_decode_op_t.u4_num_bytes_consumed = ps_ip->s_ivd_video_decode_ip_t.u4_num_Bytes;
 }
    ps_op->s_ivd_video_decode_op_t.u4_pic_ht = ps_dec->u2_vertical_size;
    ps_op->s_ivd_video_decode_op_t.u4_pic_wd = ps_dec->u2_horizontal_size;

 switch(ps_dec->e_pic_type)
 {
 case I_PIC :
            ps_op->s_ivd_video_decode_op_t.e_pic_type = IV_I_FRAME;
 break;

 case P_PIC:
            ps_op->s_ivd_video_decode_op_t.e_pic_type = IV_P_FRAME;
 break;

 case B_PIC:
            ps_op->s_ivd_video_decode_op_t.e_pic_type = IV_B_FRAME;
 break;

 case D_PIC:
            ps_op->s_ivd_video_decode_op_t.e_pic_type = IV_I_FRAME;
 break;

 default :
            ps_op->s_ivd_video_decode_op_t.e_pic_type = IV_FRAMETYPE_DEFAULT;
 break;
 }

        ps_op->s_ivd_video_decode_op_t.u4_frame_decoded_flag = ps_dec->i4_frame_decoded;
        ps_op->s_ivd_video_decode_op_t.u4_new_seq = 0;
        ps_op->s_ivd_video_decode_op_t.u4_error_code = ps_dec->u4_error_code;


}
