bool venc_dev::venc_set_perf_level(QOMX_VIDEO_PERF_LEVEL ePerfLevel)
{
 bool status = true;
 struct v4l2_control control;
 int rc = 0;
    control.id = V4L2_CID_MPEG_VIDC_SET_PERF_LEVEL;

 switch (ePerfLevel) {
 case OMX_QCOM_PerfLevelNominal:
        control.value = V4L2_CID_MPEG_VIDC_PERF_LEVEL_NOMINAL;
 break;
 case OMX_QCOM_PerfLevelTurbo:
        control.value = V4L2_CID_MPEG_VIDC_PERF_LEVEL_TURBO;
 break;
 default:
        status = false;
 break;
 }

 if (status) {
        DEBUG_PRINT_LOW("Calling IOCTL set control for id=%d, val=%d", control.id, control.value);
        rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);

 if (rc) {
            DEBUG_PRINT_ERROR("Failed to set control for id=%d, val=%d", control.id, control.value);
 return false;
 }

        DEBUG_PRINT_LOW("Success IOCTL set control for id=%d, value=%d", control.id, control.value);
 }
 return status;
}
