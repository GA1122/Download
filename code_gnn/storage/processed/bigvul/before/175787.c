WORD32 ih264d_get_version(iv_obj_t *dec_hdl, void *pv_api_ip, void *pv_api_op)
{
 char version_string[MAXVERSION_STRLEN + 1];
    UWORD32 version_string_len;

 ivd_ctl_getversioninfo_ip_t *ps_ip;
 ivd_ctl_getversioninfo_op_t *ps_op;

    ps_ip = (ivd_ctl_getversioninfo_ip_t *)pv_api_ip;
    ps_op = (ivd_ctl_getversioninfo_op_t *)pv_api_op;
    UNUSED(dec_hdl);
    ps_op->u4_error_code = IV_SUCCESS;

    VERSION(version_string, CODEC_NAME, CODEC_RELEASE_TYPE, CODEC_RELEASE_VER,
            CODEC_VENDOR);

 if((WORD32)ps_ip->u4_version_buffer_size <= 0)
 {
        ps_op->u4_error_code = IH264D_VERS_BUF_INSUFFICIENT;
 return (IV_FAIL);
 }

    version_string_len = strlen(version_string) + 1;

 if(ps_ip->u4_version_buffer_size >= version_string_len)  
 {
        memcpy(ps_ip->pv_version_buffer, version_string, version_string_len);
        ps_op->u4_error_code = IV_SUCCESS;
 }
 else
 {
        ps_op->u4_error_code = IH264D_VERS_BUF_INSUFFICIENT;
 return IV_FAIL;
 }
 return (IV_SUCCESS);
}
