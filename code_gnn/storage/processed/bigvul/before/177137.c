OMX_ERRORTYPE SoftAVC::setEncMode(IVE_ENC_MODE_T e_enc_mode) {
    IV_STATUS_T status;
 ive_ctl_set_enc_mode_ip_t s_enc_mode_ip;
 ive_ctl_set_enc_mode_op_t s_enc_mode_op;

    s_enc_mode_ip.e_cmd = IVE_CMD_VIDEO_CTL;
    s_enc_mode_ip.e_sub_cmd = IVE_CMD_CTL_SET_ENC_MODE;

    s_enc_mode_ip.e_enc_mode = e_enc_mode;

    s_enc_mode_ip.u4_timestamp_high = -1;
    s_enc_mode_ip.u4_timestamp_low = -1;

    s_enc_mode_ip.u4_size = sizeof(ive_ctl_set_enc_mode_ip_t);
    s_enc_mode_op.u4_size = sizeof(ive_ctl_set_enc_mode_op_t);

    status = ive_api_function(mCodecCtx, &s_enc_mode_ip, &s_enc_mode_op);
 if (status != IV_SUCCESS) {
        ALOGE("Unable to set in header encode mode = 0x%x\n",
                s_enc_mode_op.u4_error_code);
 return OMX_ErrorUndefined;
 }
 return OMX_ErrorNone;
}
