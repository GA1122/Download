OMX_ERRORTYPE SoftAVC::setGopParams() {
    IV_STATUS_T status;
 ive_ctl_set_gop_params_ip_t s_gop_params_ip;
 ive_ctl_set_gop_params_op_t s_gop_params_op;

    s_gop_params_ip.e_cmd = IVE_CMD_VIDEO_CTL;
    s_gop_params_ip.e_sub_cmd = IVE_CMD_CTL_SET_GOP_PARAMS;

    s_gop_params_ip.u4_i_frm_interval = mIInterval;
    s_gop_params_ip.u4_idr_frm_interval = mIDRInterval;

    s_gop_params_ip.u4_timestamp_high = -1;
    s_gop_params_ip.u4_timestamp_low = -1;

    s_gop_params_ip.u4_size = sizeof(ive_ctl_set_gop_params_ip_t);
    s_gop_params_op.u4_size = sizeof(ive_ctl_set_gop_params_op_t);

    status = ive_api_function(mCodecCtx, &s_gop_params_ip, &s_gop_params_op);
 if (status != IV_SUCCESS) {
        ALOGE("Unable to set ME params = 0x%x\n",
                s_gop_params_op.u4_error_code);
 return OMX_ErrorUndefined;
 }
 return OMX_ErrorNone;
}
