WORD32 ih264d_reset(iv_obj_t *dec_hdl, void *pv_api_ip, void *pv_api_op)
{
 dec_struct_t * ps_dec;
 ivd_ctl_reset_op_t *ps_ctl_op = (ivd_ctl_reset_op_t *)pv_api_op;
    UNUSED(pv_api_ip);
    ps_ctl_op->u4_error_code = 0;

    ps_dec = (dec_struct_t *)(dec_hdl->pv_codec_handle);

 if(ps_dec != NULL)
 {
        ih264d_init_decoder(ps_dec);
 }
 else
 {
        H264_DEC_DEBUG_PRINT(
 "\nReset called without Initializing the decoder\n");
        ps_ctl_op->u4_error_code = ERROR_INIT_NOT_DONE;
 }

 return IV_SUCCESS;
}
