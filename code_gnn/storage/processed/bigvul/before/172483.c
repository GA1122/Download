bool venc_dev::venc_set_target_bitrate(OMX_U32 nTargetBitrate, OMX_U32 config)
{
    DEBUG_PRINT_LOW("venc_set_target_bitrate: bitrate = %u",
 (unsigned int)nTargetBitrate);
 struct v4l2_control control;
 int rc = 0;
    control.id = V4L2_CID_MPEG_VIDEO_BITRATE;
    control.value = nTargetBitrate;

    DEBUG_PRINT_LOW("Calling IOCTL set control for id=%d, val=%d", control.id, control.value);
    rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);

 if (rc) {
        DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }

    DEBUG_PRINT_LOW("Success IOCTL set control for id=%d, value=%d", control.id, control.value);


    m_sVenc_cfg.targetbitrate = control.value;
    bitrate.target_bitrate = control.value;

 if (!config) {
        m_level_set = false;

 if (venc_set_profile_level(0, 0)) {
            DEBUG_PRINT_HIGH("Calling set level (Bitrate) with %lu",profile_level.level);
 }
 }

 return true;
}
