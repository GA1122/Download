WORD32 ihevcd_reset(iv_obj_t *ps_codec_obj, void *pv_api_ip, void *pv_api_op)
{
 codec_t *ps_codec;
 ivd_ctl_reset_op_t *s_ctl_reset_op = (ivd_ctl_reset_op_t *)pv_api_op;
    UNUSED(pv_api_ip);
    ps_codec = (codec_t *)(ps_codec_obj->pv_codec_handle);

 if(ps_codec != NULL)
 {
        DEBUG("\nReset called \n");
        ihevcd_init(ps_codec);
 }
 else
 {
        DEBUG("\nReset called without Initializing the decoder\n");
        s_ctl_reset_op->u4_error_code = IHEVCD_INIT_NOT_DONE;
 }

 return IV_SUCCESS;
}
