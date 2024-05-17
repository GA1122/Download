WORD32 ihevcd_set_params(iv_obj_t *ps_codec_obj,
 void *pv_api_ip,
 void *pv_api_op)
{

 codec_t *ps_codec;
    WORD32 ret = IV_SUCCESS;
    WORD32 strd;
 ivd_ctl_set_config_ip_t *s_ctl_dynparams_ip =
 (ivd_ctl_set_config_ip_t *)pv_api_ip;
 ivd_ctl_set_config_op_t *s_ctl_dynparams_op =
 (ivd_ctl_set_config_op_t *)pv_api_op;

    ps_codec = (codec_t *)(ps_codec_obj->pv_codec_handle);

    s_ctl_dynparams_op->u4_error_code = 0;

    ps_codec->e_pic_skip_mode = s_ctl_dynparams_ip->e_frm_skip_mode;

 if(s_ctl_dynparams_ip->e_frm_skip_mode != IVD_SKIP_NONE)
 {

 if((s_ctl_dynparams_ip->e_frm_skip_mode != IVD_SKIP_P) &&
 (s_ctl_dynparams_ip->e_frm_skip_mode != IVD_SKIP_B) &&
 (s_ctl_dynparams_ip->e_frm_skip_mode != IVD_SKIP_PB))
 {
            s_ctl_dynparams_op->u4_error_code = (1 << IVD_UNSUPPORTEDPARAM);
            ret = IV_FAIL;
 }
 }

    strd = ps_codec->i4_disp_strd;
 if(1 == ps_codec->i4_share_disp_buf)
 {
        strd = ps_codec->i4_strd;
 }


 {
 if((WORD32)s_ctl_dynparams_ip->u4_disp_wd >= ps_codec->i4_disp_wd)
 {
            strd = s_ctl_dynparams_ip->u4_disp_wd;
 }
 else if(0 == ps_codec->i4_sps_done)
 {
            strd = s_ctl_dynparams_ip->u4_disp_wd;
 }
 else if(s_ctl_dynparams_ip->u4_disp_wd == 0)
 {
            strd = ps_codec->i4_disp_strd;
 }
 else
 {
            strd = 0;
            s_ctl_dynparams_op->u4_error_code |= (1 << IVD_UNSUPPORTEDPARAM);
            s_ctl_dynparams_op->u4_error_code |= IHEVCD_INVALID_DISP_STRD;
            ret = IV_FAIL;
 }
 }

    ps_codec->i4_disp_strd = strd;
 if(1 == ps_codec->i4_share_disp_buf)
 {
        ps_codec->i4_strd = strd;
 }

 if(s_ctl_dynparams_ip->e_vid_dec_mode == IVD_DECODE_FRAME)
        ps_codec->i4_header_mode = 0;
 else if(s_ctl_dynparams_ip->e_vid_dec_mode == IVD_DECODE_HEADER)
        ps_codec->i4_header_mode = 1;
 else
 {

        s_ctl_dynparams_op->u4_error_code = (1 << IVD_UNSUPPORTEDPARAM);
        ps_codec->i4_header_mode = 1;
        ret = IV_FAIL;
 }

    ps_codec->e_frm_out_mode = IVD_DISPLAY_FRAME_OUT;

 if((s_ctl_dynparams_ip->e_frm_out_mode != IVD_DECODE_FRAME_OUT) &&
 (s_ctl_dynparams_ip->e_frm_out_mode != IVD_DISPLAY_FRAME_OUT))
 {
        s_ctl_dynparams_op->u4_error_code = (1 << IVD_UNSUPPORTEDPARAM);
        ret = IV_FAIL;
 }
    ps_codec->e_frm_out_mode = s_ctl_dynparams_ip->e_frm_out_mode;

 return ret;

}
