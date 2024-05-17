OMX_ERRORTYPE SoftAVC::setDeblockParams() {
    IV_STATUS_T status;
 ive_ctl_set_deblock_params_ip_t s_deblock_params_ip;
 ive_ctl_set_deblock_params_op_t s_deblock_params_op;

    s_deblock_params_ip.e_cmd = IVE_CMD_VIDEO_CTL;
    s_deblock_params_ip.e_sub_cmd = IVE_CMD_CTL_SET_DEBLOCK_PARAMS;

    s_deblock_params_ip.u4_disable_deblock_level = mDisableDeblkLevel;

    s_deblock_params_ip.u4_timestamp_high = -1;
    s_deblock_params_ip.u4_timestamp_low = -1;

    s_deblock_params_ip.u4_size = sizeof(ive_ctl_set_deblock_params_ip_t);
    s_deblock_params_op.u4_size = sizeof(ive_ctl_set_deblock_params_op_t);

    status = ive_api_function(mCodecCtx, &s_deblock_params_ip, &s_deblock_params_op);
 if (status != IV_SUCCESS) {
        ALOGE("Unable to enable/disable deblock params = 0x%x\n",
                s_deblock_params_op.u4_error_code);
 return OMX_ErrorUndefined;
 }
 return OMX_ErrorNone;
}
