bool venc_dev::venc_set_peak_bitrate(OMX_U32 nPeakBitrate)
{
 struct v4l2_control control;
 int rc = 0;
    control.id = V4L2_CID_MPEG_VIDEO_BITRATE_PEAK;
    control.value = nPeakBitrate;

    DEBUG_PRINT_LOW("venc_set_peak_bitrate: bitrate = %u", (unsigned int)nPeakBitrate);

    DEBUG_PRINT_LOW("Calling IOCTL set control for id=%d, val=%d", control.id, control.value);
    rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);

 if (rc) {
        DEBUG_PRINT_ERROR("Failed to set peak bitrate control");
 return false;
 }

    DEBUG_PRINT_LOW("Success IOCTL set control for id=%d, value=%d", control.id, control.value);

 return true;
}
