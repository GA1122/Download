WORD32 ih264d_set_params(iv_obj_t *dec_hdl, void *pv_api_ip, void *pv_api_op)
{

 dec_struct_t * ps_dec;
    WORD32 ret = IV_SUCCESS;

 ivd_ctl_set_config_ip_t *ps_ctl_ip =
 (ivd_ctl_set_config_ip_t *)pv_api_ip;
 ivd_ctl_set_config_op_t *ps_ctl_op =
 (ivd_ctl_set_config_op_t *)pv_api_op;

    ps_dec = (dec_struct_t *)(dec_hdl->pv_codec_handle);

    ps_dec->u4_skip_frm_mask = 0;

    ps_ctl_op->u4_error_code = 0;

    ps_dec->i4_app_skip_mode = ps_ctl_ip->e_frm_skip_mode;

  

 if(ps_ctl_ip->e_frm_skip_mode != IVD_SKIP_NONE)
 {

 if(ps_ctl_ip->e_frm_skip_mode == IVD_SKIP_P)
            ps_dec->u4_skip_frm_mask |= 1 << P_SLC_BIT;
 else if(ps_ctl_ip->e_frm_skip_mode == IVD_SKIP_B)
            ps_dec->u4_skip_frm_mask |= 1 << B_SLC_BIT;
 else if(ps_ctl_ip->e_frm_skip_mode == IVD_SKIP_PB)
 {
            ps_dec->u4_skip_frm_mask |= 1 << B_SLC_BIT;
            ps_dec->u4_skip_frm_mask |= 1 << P_SLC_BIT;
 }
 else if(ps_ctl_ip->e_frm_skip_mode == IVD_SKIP_I)
            ps_dec->u4_skip_frm_mask |= 1 << I_SLC_BIT;
 else
 {
            ps_ctl_op->u4_error_code = (1 << IVD_UNSUPPORTEDPARAM);
            ret = IV_FAIL;
 }
 }

 if((0 != ps_dec->u4_app_disp_width)
 && (ps_ctl_ip->u4_disp_wd
 != ps_dec->u4_app_disp_width))
 {
        ps_ctl_op->u4_error_code |= (1 << IVD_UNSUPPORTEDPARAM);
        ps_ctl_op->u4_error_code |= ERROR_DISP_WIDTH_INVALID;
        ret = IV_FAIL;
 }
 else
 {
 if((ps_ctl_ip->u4_disp_wd >= ps_dec->u2_pic_wd) )
 {
            ps_dec->u4_app_disp_width = ps_ctl_ip->u4_disp_wd;
 }
 else if((0 == ps_dec->i4_header_decoded)  )
 {
            ps_dec->u4_app_disp_width = ps_ctl_ip->u4_disp_wd;
 }
 else if(ps_ctl_ip->u4_disp_wd == 0)
 {
            ps_dec->u4_app_disp_width = 0;
 }
 else
 {
  
            ps_dec->u4_app_disp_width = 0;
            ps_ctl_op->u4_error_code |= (1 << IVD_UNSUPPORTEDPARAM);
            ps_ctl_op->u4_error_code |= ERROR_DISP_WIDTH_INVALID;
            ret = IV_FAIL;
 }
 }
 if(ps_ctl_ip->e_vid_dec_mode == IVD_DECODE_FRAME)
        ps_dec->i4_decode_header = 0;
 else if(ps_ctl_ip->e_vid_dec_mode == IVD_DECODE_HEADER)
        ps_dec->i4_decode_header = 1;
 else
 {
        ps_ctl_op->u4_error_code = (1 << IVD_UNSUPPORTEDPARAM);
        ps_dec->i4_decode_header = 1;
        ret = IV_FAIL;
 }

 return ret;

}
