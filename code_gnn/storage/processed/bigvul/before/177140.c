OMX_ERRORTYPE SoftAVC::setFrameType(IV_PICTURE_CODING_TYPE_T e_frame_type) {
 ive_ctl_set_frame_type_ip_t s_frame_type_ip;
 ive_ctl_set_frame_type_op_t s_frame_type_op;
    IV_STATUS_T status;
    s_frame_type_ip.e_cmd = IVE_CMD_VIDEO_CTL;
    s_frame_type_ip.e_sub_cmd = IVE_CMD_CTL_SET_FRAMETYPE;

    s_frame_type_ip.e_frame_type = e_frame_type;

    s_frame_type_ip.u4_timestamp_high = -1;
    s_frame_type_ip.u4_timestamp_low = -1;

    s_frame_type_ip.u4_size = sizeof(ive_ctl_set_frame_type_ip_t);
    s_frame_type_op.u4_size = sizeof(ive_ctl_set_frame_type_op_t);

    status = ive_api_function(mCodecCtx, &s_frame_type_ip, &s_frame_type_op);
 if (status != IV_SUCCESS) {
        ALOGE("Unable to set frame type = 0x%x\n",
                s_frame_type_op.u4_error_code);
 return OMX_ErrorUndefined;
 }
 return OMX_ErrorNone;
}
