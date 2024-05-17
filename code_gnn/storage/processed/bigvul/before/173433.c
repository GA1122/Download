OMX_ERRORTYPE omx_vdec::enable_smoothstreaming() {
 struct v4l2_control control;
 struct v4l2_format fmt;
    control.id = V4L2_CID_MPEG_VIDC_VIDEO_CONTINUE_DATA_TRANSFER;
    control.value = 1;
 int rc = ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL,&control);
 if (rc < 0) {
        DEBUG_PRINT_ERROR("Failed to enable Smooth Streaming on driver.");
 return OMX_ErrorHardware;
 }
    m_smoothstreaming_mode = true;
 return OMX_ErrorNone;
}
