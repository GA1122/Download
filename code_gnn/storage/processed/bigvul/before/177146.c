OMX_ERRORTYPE SoftAVC::setVbvParams() {
 ive_ctl_set_vbv_params_ip_t s_vbv_ip;
 ive_ctl_set_vbv_params_op_t s_vbv_op;
    IV_STATUS_T status;

    s_vbv_ip.e_cmd = IVE_CMD_VIDEO_CTL;
    s_vbv_ip.e_sub_cmd = IVE_CMD_CTL_SET_VBV_PARAMS;

    s_vbv_ip.u4_vbv_buf_size = 0;
    s_vbv_ip.u4_vbv_buffer_delay = 1000;

    s_vbv_ip.u4_timestamp_high = -1;
    s_vbv_ip.u4_timestamp_low = -1;

    s_vbv_ip.u4_size = sizeof(ive_ctl_set_vbv_params_ip_t);
    s_vbv_op.u4_size = sizeof(ive_ctl_set_vbv_params_op_t);

    status = ive_api_function(mCodecCtx, &s_vbv_ip, &s_vbv_op);
 if (status != IV_SUCCESS) {
        ALOGE("Unable to set VBC params = 0x%x\n", s_vbv_op.u4_error_code);
 return OMX_ErrorUndefined;
 }
 return OMX_ErrorNone;
}
