IV_API_CALL_STATUS_T impeg2d_api_rel_display_frame(iv_obj_t *ps_dechdl,
 void *pv_api_ip,
 void *pv_api_op)
{

 ivd_rel_display_frame_ip_t *dec_rel_disp_ip;
 ivd_rel_display_frame_op_t *dec_rel_disp_op;

 dec_state_t *ps_dec_state;
 dec_state_multi_core_t *ps_dec_state_multi_core;


    dec_rel_disp_ip = (ivd_rel_display_frame_ip_t *)pv_api_ip;
    dec_rel_disp_op = (ivd_rel_display_frame_op_t *)pv_api_op;

    dec_rel_disp_op->u4_error_code = 0;
    ps_dec_state_multi_core = (dec_state_multi_core_t *) (ps_dechdl->pv_codec_handle);
    ps_dec_state = ps_dec_state_multi_core->ps_dec_state[0];


  
 if(0 == ps_dec_state->u4_share_disp_buf)
 return IV_SUCCESS;

 if(NULL == ps_dec_state->pv_pic_buf_mg)
 return IV_SUCCESS;


    impeg2_buf_mgr_release(ps_dec_state->pv_pic_buf_mg, dec_rel_disp_ip->u4_disp_buf_id, BUF_MGR_DISP);

 return IV_SUCCESS;
}
