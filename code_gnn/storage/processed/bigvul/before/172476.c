bool venc_dev::venc_set_perf_mode(OMX_U32 mode)
{
 struct v4l2_control control;
 if (mode && mode <= V4L2_MPEG_VIDC_VIDEO_PERF_POWER_SAVE) {
        control.id = V4L2_CID_MPEG_VIDC_VIDEO_PERF_MODE;
        control.value = mode;
        DEBUG_PRINT_LOW("Going to set V4L2_CID_MPEG_VIDC_VIDEO_PERF_MODE");
 if (ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control)) {
            DEBUG_PRINT_ERROR("Failed to set V4L2_CID_MPEG_VIDC_VIDEO_PERF_MODE");
 return false;
 }
 return true;
 } else {
        DEBUG_PRINT_ERROR("Invalid mode set for V4L2_CID_MPEG_VIDC_VIDEO_PERF_MODE: %d", mode);
 return false;
 }
}
