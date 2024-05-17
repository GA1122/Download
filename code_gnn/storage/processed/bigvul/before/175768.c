IV_API_CALL_STATUS_T impeg2d_api_set_flush_mode(iv_obj_t *ps_dechdl,
 void *pv_api_ip,
 void *pv_api_op)
{
 dec_state_t *ps_dec_state;
 dec_state_multi_core_t *ps_dec_state_multi_core;
 impeg2d_ctl_flush_op_t *ps_ctl_dec_op =
 (impeg2d_ctl_flush_op_t*)pv_api_op;

    UNUSED(pv_api_ip);

    ps_dec_state_multi_core =
 (dec_state_multi_core_t *)(ps_dechdl->pv_codec_handle);
    ps_dec_state = ps_dec_state_multi_core->ps_dec_state[0];

    ps_dec_state->u1_flushfrm = 1;

    ps_ctl_dec_op->s_ivd_ctl_flush_op_t.u4_size =
 sizeof(impeg2d_ctl_flush_op_t);
    ps_ctl_dec_op->s_ivd_ctl_flush_op_t.u4_error_code = IV_SUCCESS;

 return (IV_SUCCESS);
}
