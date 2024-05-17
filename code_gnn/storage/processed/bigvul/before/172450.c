bool venc_dev::venc_set_au_delimiter(OMX_BOOL enable)
{
 struct v4l2_control control;

    control.id = V4L2_CID_MPEG_VIDC_VIDEO_H264_AU_DELIMITER;
 if(enable) {
        control.value = V4L2_MPEG_VIDC_VIDEO_H264_AU_DELIMITER_ENABLED;
 } else {
        control.value = V4L2_MPEG_VIDC_VIDEO_H264_AU_DELIMITER_DISABLED;
 }

    DEBUG_PRINT_HIGH("Set au delimiter: %d", enable);
 if(ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control) < 0) {
        DEBUG_PRINT_ERROR("Request to set AU delimiter failed");
 return false;
 }
 return true;
}
