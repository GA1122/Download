IV_API_CALL_STATUS_T impeg2d_api_set_display_frame(iv_obj_t *ps_dechdl,
 void *pv_api_ip,
 void *pv_api_op)
{

 ivd_set_display_frame_ip_t *dec_disp_ip;
 ivd_set_display_frame_op_t *dec_disp_op;

    UWORD32 i;
 dec_state_t *ps_dec_state;
 dec_state_multi_core_t *ps_dec_state_multi_core;
    UWORD32 u4_num_disp_bufs;


    dec_disp_ip = (ivd_set_display_frame_ip_t *)pv_api_ip;
    dec_disp_op = (ivd_set_display_frame_op_t *)pv_api_op;
    dec_disp_op->u4_error_code = 0;

    u4_num_disp_bufs = dec_disp_ip->num_disp_bufs;
 if(u4_num_disp_bufs > BUF_MGR_MAX_CNT)
        u4_num_disp_bufs = BUF_MGR_MAX_CNT;

    ps_dec_state_multi_core = (dec_state_multi_core_t *) (ps_dechdl->pv_codec_handle);
    ps_dec_state = ps_dec_state_multi_core->ps_dec_state[0];

 if(ps_dec_state->u4_share_disp_buf)
 {
 pic_buf_t *ps_pic_buf;
        ps_pic_buf = (pic_buf_t *)ps_dec_state->pv_pic_buf_base;
 for(i = 0; i < u4_num_disp_bufs; i++)
 {

            ps_pic_buf->pu1_y = dec_disp_ip->s_disp_buffer[i].pu1_bufs[0];
 if(IV_YUV_420P == ps_dec_state->i4_chromaFormat)
 {
                ps_pic_buf->pu1_u = dec_disp_ip->s_disp_buffer[i].pu1_bufs[1];
                ps_pic_buf->pu1_v = dec_disp_ip->s_disp_buffer[i].pu1_bufs[2];
 }
 else
 {
                ps_pic_buf->pu1_u = ps_dec_state->pu1_chroma_ref_buf[i];
                ps_pic_buf->pu1_v = ps_dec_state->pu1_chroma_ref_buf[i] +
 ((ps_dec_state->u2_create_max_width * ps_dec_state->u2_create_max_height) >> 2);
 }

            ps_pic_buf->i4_buf_id = i;

            ps_pic_buf->u1_used_as_ref = 0;

            ps_pic_buf->u4_ts = 0;

            impeg2_buf_mgr_add(ps_dec_state->pv_pic_buf_mg, ps_pic_buf, i);
            impeg2_buf_mgr_set_status(ps_dec_state->pv_pic_buf_mg, i, BUF_MGR_DISP);
            ps_pic_buf++;

 }
 }
    memcpy(&(ps_dec_state->as_disp_buffers[0]),
 &(dec_disp_ip->s_disp_buffer),
           u4_num_disp_bufs * sizeof(ivd_out_bufdesc_t));

 return IV_SUCCESS;

}
