void SoftAVC::logVersion() {
 ive_ctl_getversioninfo_ip_t s_ctl_ip;
 ive_ctl_getversioninfo_op_t s_ctl_op;
    UWORD8 au1_buf[512];
    IV_STATUS_T status;

    s_ctl_ip.e_cmd = IVE_CMD_VIDEO_CTL;
    s_ctl_ip.e_sub_cmd = IVE_CMD_CTL_GETVERSION;
    s_ctl_ip.u4_size = sizeof(ive_ctl_getversioninfo_ip_t);
    s_ctl_op.u4_size = sizeof(ive_ctl_getversioninfo_op_t);
    s_ctl_ip.pu1_version = au1_buf;
    s_ctl_ip.u4_version_bufsize = sizeof(au1_buf);

    status = ive_api_function(mCodecCtx, (void *) &s_ctl_ip, (void *) &s_ctl_op);

 if (status != IV_SUCCESS) {
        ALOGE("Error in getting version: 0x%x", s_ctl_op.u4_error_code);
 } else {
        ALOGV("Ittiam encoder version: %s", (char *)s_ctl_ip.pu1_version);
 }
 return;
}
