status_t SoftHEVC::resetDecoder() {
 ivd_ctl_reset_ip_t s_ctl_ip;
 ivd_ctl_reset_op_t s_ctl_op;
    IV_API_CALL_STATUS_T status;

    s_ctl_ip.e_cmd = IVD_CMD_VIDEO_CTL;
    s_ctl_ip.e_sub_cmd = IVD_CMD_CTL_RESET;
    s_ctl_ip.u4_size = sizeof(ivd_ctl_reset_ip_t);
    s_ctl_op.u4_size = sizeof(ivd_ctl_reset_op_t);

    status = ivdec_api_function(mCodecCtx, (void *)&s_ctl_ip,
 (void *)&s_ctl_op);
 if (IV_SUCCESS != status) {
        ALOGE("Error in reset: 0x%x", s_ctl_op.u4_error_code);
 return UNKNOWN_ERROR;
 }
    mSignalledError = false;

  
    setNumCores();

    mStride = 0;
 return OK;
}
