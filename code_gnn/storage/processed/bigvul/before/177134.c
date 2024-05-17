OMX_ERRORTYPE SoftAVC::setBitRate() {
 ive_ctl_set_bitrate_ip_t s_bitrate_ip;
 ive_ctl_set_bitrate_op_t s_bitrate_op;
    IV_STATUS_T status;

    s_bitrate_ip.e_cmd = IVE_CMD_VIDEO_CTL;
    s_bitrate_ip.e_sub_cmd = IVE_CMD_CTL_SET_BITRATE;

    s_bitrate_ip.u4_target_bitrate = mBitrate;

    s_bitrate_ip.u4_timestamp_high = -1;
    s_bitrate_ip.u4_timestamp_low = -1;

    s_bitrate_ip.u4_size = sizeof(ive_ctl_set_bitrate_ip_t);
    s_bitrate_op.u4_size = sizeof(ive_ctl_set_bitrate_op_t);

    status = ive_api_function(mCodecCtx, &s_bitrate_ip, &s_bitrate_op);
 if (status != IV_SUCCESS) {
        ALOGE("Unable to set bit rate = 0x%x\n", s_bitrate_op.u4_error_code);
 return OMX_ErrorUndefined;
 }
 return OMX_ErrorNone;
}
