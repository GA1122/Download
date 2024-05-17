OMX_ERRORTYPE SoftAVC::setIpeParams() {
 ive_ctl_set_ipe_params_ip_t s_ipe_params_ip;
 ive_ctl_set_ipe_params_op_t s_ipe_params_op;
    IV_STATUS_T status;

    s_ipe_params_ip.e_cmd = IVE_CMD_VIDEO_CTL;
    s_ipe_params_ip.e_sub_cmd = IVE_CMD_CTL_SET_IPE_PARAMS;

    s_ipe_params_ip.u4_enable_intra_4x4 = mIntra4x4;
    s_ipe_params_ip.u4_enc_speed_preset = mEncSpeed;

    s_ipe_params_ip.u4_timestamp_high = -1;
    s_ipe_params_ip.u4_timestamp_low = -1;

    s_ipe_params_ip.u4_size = sizeof(ive_ctl_set_ipe_params_ip_t);
    s_ipe_params_op.u4_size = sizeof(ive_ctl_set_ipe_params_op_t);

    status = ive_api_function(mCodecCtx, &s_ipe_params_ip, &s_ipe_params_op);
 if (status != IV_SUCCESS) {
        ALOGE("Unable to set ipe params = 0x%x\n",
                s_ipe_params_op.u4_error_code);
 return OMX_ErrorUndefined;
 }
 return OMX_ErrorNone;
}
