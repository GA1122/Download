OMX_ERRORTYPE SoftAVC::setMeParams() {
    IV_STATUS_T status;
 ive_ctl_set_me_params_ip_t s_me_params_ip;
 ive_ctl_set_me_params_op_t s_me_params_op;

    s_me_params_ip.e_cmd = IVE_CMD_VIDEO_CTL;
    s_me_params_ip.e_sub_cmd = IVE_CMD_CTL_SET_ME_PARAMS;

    s_me_params_ip.u4_enable_fast_sad = mEnableFastSad;
    s_me_params_ip.u4_enable_alt_ref = mEnableAltRef;

    s_me_params_ip.u4_enable_hpel = mHalfPelEnable;
    s_me_params_ip.u4_enable_qpel = DEFAULT_QPEL;
    s_me_params_ip.u4_me_speed_preset = DEFAULT_ME_SPEED;
    s_me_params_ip.u4_srch_rng_x = DEFAULT_SRCH_RNG_X;
    s_me_params_ip.u4_srch_rng_y = DEFAULT_SRCH_RNG_Y;

    s_me_params_ip.u4_timestamp_high = -1;
    s_me_params_ip.u4_timestamp_low = -1;

    s_me_params_ip.u4_size = sizeof(ive_ctl_set_me_params_ip_t);
    s_me_params_op.u4_size = sizeof(ive_ctl_set_me_params_op_t);

    status = ive_api_function(mCodecCtx, &s_me_params_ip, &s_me_params_op);
 if (status != IV_SUCCESS) {
        ALOGE("Unable to set me params = 0x%x\n", s_me_params_op.u4_error_code);
 return OMX_ErrorUndefined;
 }
 return OMX_ErrorNone;
}
