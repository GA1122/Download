IV_API_CALL_STATUS_T impeg2d_api_get_buf_info(iv_obj_t *ps_dechdl,
 void *pv_api_ip,
 void *pv_api_op)
{
 dec_state_t *ps_dec_state;
 dec_state_multi_core_t *ps_dec_state_multi_core;
 impeg2d_ctl_getbufinfo_ip_t *ps_ctl_bufinfo_ip =
 (impeg2d_ctl_getbufinfo_ip_t *)pv_api_ip;
 impeg2d_ctl_getbufinfo_op_t *ps_ctl_bufinfo_op =
 (impeg2d_ctl_getbufinfo_op_t *)pv_api_op;
    UWORD32 u4_i, u4_stride, u4_height;
    UNUSED(ps_ctl_bufinfo_ip);

    ps_dec_state_multi_core =
 (dec_state_multi_core_t *)(ps_dechdl->pv_codec_handle);
    ps_dec_state = ps_dec_state_multi_core->ps_dec_state[0];

    ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_num_in_bufs = 1;
    ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_num_out_bufs = 1;

 if(ps_dec_state->i4_chromaFormat == IV_YUV_420P)
 {
        ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_num_out_bufs =
                        MIN_OUT_BUFS_420;
 }
 else if((ps_dec_state->i4_chromaFormat == IV_YUV_420SP_UV)
 || (ps_dec_state->i4_chromaFormat == IV_YUV_420SP_VU))
 {
        ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_num_out_bufs =
                        MIN_OUT_BUFS_420SP;
 }
 else if(ps_dec_state->i4_chromaFormat == IV_YUV_422ILE)
 {
        ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_num_out_bufs =
                        MIN_OUT_BUFS_422ILE;
 }
 else if(ps_dec_state->i4_chromaFormat == IV_RGB_565)
 {
        ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_num_out_bufs =
                        MIN_OUT_BUFS_RGB565;
 }
 else
 {
        ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_error_code =
                        IVD_INIT_DEC_COL_FMT_NOT_SUPPORTED;
 return IV_FAIL;
 }

 for(u4_i = 0; u4_i < IVD_VIDDEC_MAX_IO_BUFFERS; u4_i++)
 {
        ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_in_buf_size[u4_i] =
 0;
        ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_out_buf_size[u4_i] =
 0;
 }

 for(u4_i = 0;
        u4_i < ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_num_in_bufs;
        u4_i++)
 {
        ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_in_buf_size[u4_i] =
                        MAX_BITSTREAM_BUFFER_SIZE;
 }

 if (0 == ps_dec_state->u4_frm_buf_stride)
 {
 if (1 == ps_dec_state->u2_header_done)
 {
            u4_stride   = ps_dec_state->u2_horizontal_size;
 }
 else
 {
            u4_stride   = ps_dec_state->u2_create_max_width;
 }
 }
 else
 {
        u4_stride = ps_dec_state->u4_frm_buf_stride;
 }
    u4_height = ((ps_dec_state->u2_frame_height + 15) >> 4) << 4;

 if(ps_dec_state->i4_chromaFormat == IV_YUV_420P)
 {
        ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_out_buf_size[0] =
 (u4_stride * u4_height);
        ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_out_buf_size[1] =
 (u4_stride * u4_height) >> 2;
        ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_out_buf_size[2] =
 (u4_stride * u4_height) >> 2;
 }
 else if((ps_dec_state->i4_chromaFormat == IV_YUV_420SP_UV)
 || (ps_dec_state->i4_chromaFormat == IV_YUV_420SP_VU))
 {
        ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_out_buf_size[0] =
 (u4_stride * u4_height);
        ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_out_buf_size[1] =
 (u4_stride * u4_height) >> 1;
        ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_out_buf_size[2] = 0;
 }
 else if(ps_dec_state->i4_chromaFormat == IV_YUV_422ILE)
 {
        ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_out_buf_size[0] =
 (u4_stride * u4_height) * 2;
        ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_out_buf_size[1] =
                        ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_min_out_buf_size[2] =
 0;
 }

  
    ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_num_disp_bufs = 1;
 if(ps_dec_state->u4_share_disp_buf)
        ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_num_disp_bufs =
                        NUM_INT_FRAME_BUFFERS;
    ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_size = MAX_FRM_SIZE;

    ps_ctl_bufinfo_op->s_ivd_ctl_getbufinfo_op_t.u4_error_code = IV_SUCCESS;

 return (IV_SUCCESS);
}
