WORD32 ih264d_delete(iv_obj_t *dec_hdl, void *pv_api_ip, void *pv_api_op)
{
 dec_struct_t *ps_dec;
 ih264d_delete_ip_t *ps_ip = (ih264d_delete_ip_t *)pv_api_ip;
 ih264d_delete_op_t *ps_op = (ih264d_delete_op_t *)pv_api_op;

    ps_dec = (dec_struct_t *)(dec_hdl->pv_codec_handle);
    UNUSED(ps_ip);
    ps_op->s_ivd_delete_op_t.u4_error_code = 0;
    ih264d_free_dynamic_bufs(ps_dec);
    ih264d_free_static_bufs(dec_hdl);
 return IV_SUCCESS;
}
