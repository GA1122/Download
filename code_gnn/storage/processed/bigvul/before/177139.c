OMX_ERRORTYPE SoftAVC::setFrameRate() {
 ive_ctl_set_frame_rate_ip_t s_frame_rate_ip;
 ive_ctl_set_frame_rate_op_t s_frame_rate_op;
    IV_STATUS_T status;

    s_frame_rate_ip.e_cmd = IVE_CMD_VIDEO_CTL;
    s_frame_rate_ip.e_sub_cmd = IVE_CMD_CTL_SET_FRAMERATE;

    s_frame_rate_ip.u4_src_frame_rate = mFramerate >> 16;
    s_frame_rate_ip.u4_tgt_frame_rate = mFramerate >> 16;

    s_frame_rate_ip.u4_timestamp_high = -1;
    s_frame_rate_ip.u4_timestamp_low = -1;

    s_frame_rate_ip.u4_size = sizeof(ive_ctl_set_frame_rate_ip_t);
    s_frame_rate_op.u4_size = sizeof(ive_ctl_set_frame_rate_op_t);

    status = ive_api_function(mCodecCtx, &s_frame_rate_ip, &s_frame_rate_op);
 if (status != IV_SUCCESS) {
        ALOGE("Unable to set frame rate = 0x%x\n",
                s_frame_rate_op.u4_error_code);
 return OMX_ErrorUndefined;
 }
 return OMX_ErrorNone;
}
