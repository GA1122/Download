IV_API_CALL_STATUS_T impeg2d_api_get_version(iv_obj_t *ps_dechdl,
 void *pv_api_ip,
 void *pv_api_op)
{
 char au1_version_string[512];

 impeg2d_ctl_getversioninfo_ip_t *ps_ip;
 impeg2d_ctl_getversioninfo_op_t *ps_op;

    UNUSED(ps_dechdl);

    ps_ip = (impeg2d_ctl_getversioninfo_ip_t *)pv_api_ip;
    ps_op = (impeg2d_ctl_getversioninfo_op_t *)pv_api_op;

    ps_op->s_ivd_ctl_getversioninfo_op_t.u4_error_code = IV_SUCCESS;

    VERSION(au1_version_string, CODEC_NAME, CODEC_RELEASE_TYPE, CODEC_RELEASE_VER,
            CODEC_VENDOR);

 if((WORD32)ps_ip->s_ivd_ctl_getversioninfo_ip_t.u4_version_buffer_size <= 0)
 {
        ps_op->s_ivd_ctl_getversioninfo_op_t.u4_error_code = IV_FAIL;
 return (IV_FAIL);
 }

 if(ps_ip->s_ivd_ctl_getversioninfo_ip_t.u4_version_buffer_size
 >= (strlen(au1_version_string) + 1))
 {
        memcpy(ps_ip->s_ivd_ctl_getversioninfo_ip_t.pv_version_buffer,
               au1_version_string, (strlen(au1_version_string) + 1));
        ps_op->s_ivd_ctl_getversioninfo_op_t.u4_error_code = IV_SUCCESS;
 }
 else
 {
        ps_op->s_ivd_ctl_getversioninfo_op_t.u4_error_code = IV_FAIL;
 }

 return (IV_SUCCESS);
}
