WORD32 ih264d_set_default_params(iv_obj_t *dec_hdl,
 void *pv_api_ip,
 void *pv_api_op)
{

 dec_struct_t * ps_dec;
    WORD32 ret = IV_SUCCESS;

 ivd_ctl_set_config_op_t *ps_ctl_op =
 (ivd_ctl_set_config_op_t *)pv_api_op;
    ps_dec = (dec_struct_t *)(dec_hdl->pv_codec_handle);
    UNUSED(pv_api_ip);


 {
        ps_dec->u4_app_disp_width = 0;
        ps_dec->u4_skip_frm_mask = 0;
        ps_dec->i4_decode_header = 1;

        ps_ctl_op->u4_error_code = 0;
 }


 return ret;
}
