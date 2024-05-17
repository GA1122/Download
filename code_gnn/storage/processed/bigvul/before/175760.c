IV_API_CALL_STATUS_T impeg2d_api_get_status(iv_obj_t *ps_dechdl,
 void *pv_api_ip,
 void *pv_api_op)
{
 dec_state_t *ps_dec_state;
 dec_state_multi_core_t *ps_dec_state_multi_core;
    UWORD32 u4_i,u4_stride,u4_height;
 impeg2d_ctl_getstatus_ip_t *ps_ctl_dec_ip = (impeg2d_ctl_getstatus_ip_t *)pv_api_ip;
 impeg2d_ctl_getstatus_op_t *ps_ctl_dec_op = (impeg2d_ctl_getstatus_op_t *)pv_api_op;
    UNUSED(ps_ctl_dec_ip);

    ps_dec_state_multi_core = (dec_state_multi_core_t *) (ps_dechdl->pv_codec_handle);
    ps_dec_state = ps_dec_state_multi_core->ps_dec_state[0];

    ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_size             = sizeof(impeg2d_ctl_getstatus_op_t);
    ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_num_disp_bufs    = 1;
    ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_pic_ht           = ps_dec_state->u2_frame_height;
    ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_pic_wd           = ps_dec_state->u2_frame_width;
    ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_frame_rate           = ps_dec_state->u2_framePeriod;


 if(ps_dec_state->u2_progressive_sequence == 1)
        ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.e_content_type          =   IV_PROGRESSIVE ;
 else
        ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.e_content_type          = IV_INTERLACED;


    ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.e_output_chroma_format  = (IV_COLOR_FORMAT_T)ps_dec_state->i4_chromaFormat;
    ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_min_num_in_bufs          = 1;
    ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_min_num_out_bufs     = 1;


 if(ps_dec_state->i4_chromaFormat == IV_YUV_420P)
 {
        ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_min_num_out_bufs     = MIN_OUT_BUFS_420;
 }
 else if(ps_dec_state->i4_chromaFormat == IV_YUV_422ILE)
 {
        ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_min_num_out_bufs     = MIN_OUT_BUFS_422ILE;
 }
 else if(ps_dec_state->i4_chromaFormat == IV_RGB_565)
 {
        ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_min_num_out_bufs = MIN_OUT_BUFS_RGB565;
 }
 else
 {
        ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_error_code   = IVD_INIT_DEC_COL_FMT_NOT_SUPPORTED;
 return IV_FAIL;
 }

    memset(&ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_min_in_buf_size[0],0,(sizeof(UWORD32)*IVD_VIDDEC_MAX_IO_BUFFERS));
    memset(&ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_min_out_buf_size[0],0,(sizeof(UWORD32)*IVD_VIDDEC_MAX_IO_BUFFERS));

 for(u4_i = 0; u4_i < ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_min_num_in_bufs; u4_i++)
 {
        ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_min_in_buf_size[u4_i] = MAX_BITSTREAM_BUFFER_SIZE;
 }

    u4_stride = ps_dec_state->u4_frm_buf_stride;
    u4_height = ((ps_dec_state->u2_frame_height + 15) >> 4) << 4;

 if(ps_dec_state->i4_chromaFormat == IV_YUV_420P)
 {
        ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_min_out_buf_size[0] = (u4_stride * u4_height);
        ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_min_out_buf_size[1] = (u4_stride * u4_height)>>2 ;
        ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_min_out_buf_size[2] = (u4_stride * u4_height)>>2;
 }
 else if((ps_dec_state->i4_chromaFormat == IV_YUV_420SP_UV) || (ps_dec_state->i4_chromaFormat == IV_YUV_420SP_VU))
 {
        ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_min_out_buf_size[0] = (u4_stride * u4_height);
        ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_min_out_buf_size[1] = (u4_stride * u4_height)>>1 ;
        ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_min_out_buf_size[2] = 0;
 }
 else if(ps_dec_state->i4_chromaFormat == IV_YUV_422ILE)
 {
        ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_min_out_buf_size[0] = (u4_stride * u4_height)*2;
        ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_min_out_buf_size[1] = ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_min_out_buf_size[2] = 0;
 }

    ps_ctl_dec_op->s_ivd_ctl_getstatus_op_t.u4_error_code = IV_SUCCESS;

 return(IV_SUCCESS);

}
