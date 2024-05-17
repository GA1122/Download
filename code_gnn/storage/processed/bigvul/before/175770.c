IV_API_CALL_STATUS_T impeg2d_api_set_params(iv_obj_t *ps_dechdl,void *pv_api_ip,void *pv_api_op)
{
 dec_state_t *ps_dec_state;
 dec_state_multi_core_t *ps_dec_state_multi_core;
 impeg2d_ctl_set_config_ip_t *ps_ctl_dec_ip = (impeg2d_ctl_set_config_ip_t *)pv_api_ip;
 impeg2d_ctl_set_config_op_t *ps_ctl_dec_op = (impeg2d_ctl_set_config_op_t *)pv_api_op;

    ps_dec_state_multi_core = (dec_state_multi_core_t *) (ps_dechdl->pv_codec_handle);
    ps_dec_state = ps_dec_state_multi_core->ps_dec_state[0];

 if((ps_ctl_dec_ip->s_ivd_ctl_set_config_ip_t.e_vid_dec_mode != IVD_DECODE_HEADER) && (ps_ctl_dec_ip->s_ivd_ctl_set_config_ip_t.e_vid_dec_mode != IVD_DECODE_FRAME))
 {
        ps_ctl_dec_op->s_ivd_ctl_set_config_op_t.u4_error_code = IV_FAIL;
 return(IV_FAIL);
 }

 if((ps_ctl_dec_ip->s_ivd_ctl_set_config_ip_t.e_frm_out_mode != IVD_DISPLAY_FRAME_OUT) && (ps_ctl_dec_ip->s_ivd_ctl_set_config_ip_t.e_frm_out_mode != IVD_DECODE_FRAME_OUT))
 {
        ps_ctl_dec_op->s_ivd_ctl_set_config_op_t.u4_error_code = IV_FAIL;
 return(IV_FAIL);
 }

 if( (WORD32) ps_ctl_dec_ip->s_ivd_ctl_set_config_ip_t.e_frm_skip_mode < IVD_SKIP_NONE)
 {
        ps_ctl_dec_op->s_ivd_ctl_set_config_op_t.u4_error_code = IV_FAIL;
 return(IV_FAIL);
 }

 if(ps_dec_state->u2_header_done == 1)
 {
 if(((WORD32)ps_ctl_dec_ip->s_ivd_ctl_set_config_ip_t.u4_disp_wd < 0) ||
 ((ps_ctl_dec_ip->s_ivd_ctl_set_config_ip_t.u4_disp_wd != 0) && (ps_ctl_dec_ip->s_ivd_ctl_set_config_ip_t.u4_disp_wd < ps_dec_state->u2_frame_width)))
 {
            ps_ctl_dec_op->s_ivd_ctl_set_config_op_t.u4_error_code = IV_FAIL;
 return(IV_FAIL);
 }

 }


    ps_dec_state->u2_decode_header    = (UWORD8)ps_ctl_dec_ip->s_ivd_ctl_set_config_ip_t.e_vid_dec_mode;

 if(ps_ctl_dec_ip->s_ivd_ctl_set_config_ip_t.u4_disp_wd != 0)
 {
 if(ps_dec_state->u2_header_done == 1)
 {
 if (ps_ctl_dec_ip->s_ivd_ctl_set_config_ip_t.u4_disp_wd > ps_dec_state->u2_frame_width)
 {
                ps_dec_state->u4_frm_buf_stride = ps_ctl_dec_ip->s_ivd_ctl_set_config_ip_t.u4_disp_wd;
 }
 }
 else
 {
            ps_dec_state->u4_frm_buf_stride = ps_ctl_dec_ip->s_ivd_ctl_set_config_ip_t.u4_disp_wd;
 }

 }
 else
 {

 if(ps_dec_state->u2_header_done == 1)
 {
                ps_dec_state->u4_frm_buf_stride = ps_dec_state->u2_frame_width;
 }
 else
 {
                ps_dec_state->u4_frm_buf_stride = 0;
 }
 }


 if(ps_ctl_dec_ip->s_ivd_ctl_set_config_ip_t.e_vid_dec_mode  == IVD_DECODE_FRAME)
 {
            ps_dec_state->u1_flushfrm = 0;
 }


    ps_ctl_dec_op->s_ivd_ctl_set_config_op_t.u4_error_code = IV_SUCCESS;
 return(IV_SUCCESS);

}
