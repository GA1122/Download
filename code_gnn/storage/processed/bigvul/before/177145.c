OMX_ERRORTYPE SoftAVC::setProfileParams() {
    IV_STATUS_T status;
 ive_ctl_set_profile_params_ip_t s_profile_params_ip;
 ive_ctl_set_profile_params_op_t s_profile_params_op;

    s_profile_params_ip.e_cmd = IVE_CMD_VIDEO_CTL;
    s_profile_params_ip.e_sub_cmd = IVE_CMD_CTL_SET_PROFILE_PARAMS;

    s_profile_params_ip.e_profile = DEFAULT_EPROFILE;
    s_profile_params_ip.u4_entropy_coding_mode = mEntropyMode;
    s_profile_params_ip.u4_timestamp_high = -1;
    s_profile_params_ip.u4_timestamp_low = -1;

    s_profile_params_ip.u4_size = sizeof(ive_ctl_set_profile_params_ip_t);
    s_profile_params_op.u4_size = sizeof(ive_ctl_set_profile_params_op_t);

    status = ive_api_function(mCodecCtx, &s_profile_params_ip, &s_profile_params_op);
 if (status != IV_SUCCESS) {
        ALOGE("Unable to set profile params = 0x%x\n",
                s_profile_params_op.u4_error_code);
 return OMX_ErrorUndefined;
 }
 return OMX_ErrorNone;
}