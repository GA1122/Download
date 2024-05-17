WORD32 ihevcd_delete(iv_obj_t *ps_codec_obj, void *pv_api_ip, void *pv_api_op)
{
 codec_t *ps_dec;
 ihevcd_cxa_delete_ip_t *ps_ip = (ihevcd_cxa_delete_ip_t *)pv_api_ip;
 ihevcd_cxa_delete_op_t *ps_op = (ihevcd_cxa_delete_op_t *)pv_api_op;

    ps_dec = (codec_t *)(ps_codec_obj->pv_codec_handle);
    UNUSED(ps_ip);
    ps_op->s_ivd_delete_op_t.u4_error_code = 0;
    ihevcd_free_dynamic_bufs(ps_dec);
    ihevcd_free_static_bufs(ps_codec_obj);
 return IV_SUCCESS;
}
