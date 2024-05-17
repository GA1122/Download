bool venc_dev::venc_set_entropy_config(OMX_BOOL enable, OMX_U32 i_cabac_level)
{
 int rc = 0;
 struct v4l2_control control;

    DEBUG_PRINT_LOW("venc_set_entropy_config: CABAC = %u level: %u", enable, (unsigned int)i_cabac_level);

 if (enable && (codec_profile.profile != V4L2_MPEG_VIDEO_H264_PROFILE_BASELINE) &&
 (codec_profile.profile != V4L2_MPEG_VIDEO_H264_PROFILE_CONSTRAINED_BASELINE)) {

        control.value = V4L2_MPEG_VIDEO_H264_ENTROPY_MODE_CABAC;
        control.id = V4L2_CID_MPEG_VIDEO_H264_ENTROPY_MODE;

        DEBUG_PRINT_LOW("Calling IOCTL set control for id=%d, val=%d", control.id, control.value);
        rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);

 if (rc) {
            DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }

        DEBUG_PRINT_LOW("Success IOCTL set control for id=%d, value=%d", control.id, control.value);
        entropy.longentropysel = control.value;

 if (i_cabac_level == 0) {
            control.value = V4L2_CID_MPEG_VIDC_VIDEO_H264_CABAC_MODEL_0;
 } else if (i_cabac_level == 1) {
            control.value = V4L2_CID_MPEG_VIDC_VIDEO_H264_CABAC_MODEL_1;
 } else if (i_cabac_level == 2) {
            control.value = V4L2_CID_MPEG_VIDC_VIDEO_H264_CABAC_MODEL_2;
 }

        control.id = V4L2_CID_MPEG_VIDC_VIDEO_H264_CABAC_MODEL;
        DEBUG_PRINT_LOW("Calling IOCTL set control for id=%d, val=%d", control.id, control.value);
        rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);

 if (rc) {
            DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }

        DEBUG_PRINT_LOW("Success IOCTL set control for id=%d, value=%d", control.id, control.value);
        entropy.cabacmodel=control.value;
 } else if (!enable) {
        control.value =  V4L2_MPEG_VIDEO_H264_ENTROPY_MODE_CAVLC;
        control.id = V4L2_CID_MPEG_VIDEO_H264_ENTROPY_MODE;
        DEBUG_PRINT_LOW("Calling IOCTL set control for id=%d, val=%d", control.id, control.value);
        rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);

 if (rc) {
            DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }

        DEBUG_PRINT_LOW("Success IOCTL set control for id=%d, value=%d", control.id, control.value);
        entropy.longentropysel=control.value;
 } else {
        DEBUG_PRINT_ERROR("Invalid Entropy mode for Baseline Profile");
 return false;
 }

 return true;
}
