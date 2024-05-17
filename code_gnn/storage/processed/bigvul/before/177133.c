OMX_ERRORTYPE SoftAVC::setAirParams() {
 ive_ctl_set_air_params_ip_t s_air_ip;
 ive_ctl_set_air_params_op_t s_air_op;
    IV_STATUS_T status;

    s_air_ip.e_cmd = IVE_CMD_VIDEO_CTL;
    s_air_ip.e_sub_cmd = IVE_CMD_CTL_SET_AIR_PARAMS;

    s_air_ip.e_air_mode = mAIRMode;
    s_air_ip.u4_air_refresh_period = mAIRRefreshPeriod;

    s_air_ip.u4_timestamp_high = -1;
    s_air_ip.u4_timestamp_low = -1;

    s_air_ip.u4_size = sizeof(ive_ctl_set_air_params_ip_t);
    s_air_op.u4_size = sizeof(ive_ctl_set_air_params_op_t);

    status = ive_api_function(mCodecCtx, &s_air_ip, &s_air_op);
 if (status != IV_SUCCESS) {
        ALOGE("Unable to set air params = 0x%x\n", s_air_op.u4_error_code);
 return OMX_ErrorUndefined;
 }
 return OMX_ErrorNone;
}
