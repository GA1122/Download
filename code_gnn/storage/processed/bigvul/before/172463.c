bool venc_dev::venc_set_inband_video_header(OMX_BOOL enable)
{
 struct v4l2_control control;

    control.id = V4L2_CID_MPEG_VIDEO_HEADER_MODE;
 if(enable) {
        control.value = V4L2_MPEG_VIDEO_HEADER_MODE_JOINED_WITH_I_FRAME;
 } else {
        control.value = V4L2_MPEG_VIDEO_HEADER_MODE_SEPARATE;
 }

    DEBUG_PRINT_HIGH("Set inband sps/pps: %d", enable);
 if(ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control) < 0) {
        DEBUG_PRINT_ERROR("Request for inband sps/pps failed");
 return false;
 }
 return true;
}
