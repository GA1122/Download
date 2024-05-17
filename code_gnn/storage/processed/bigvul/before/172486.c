bool venc_dev::venc_set_vpe_rotation(OMX_S32 rotation_angle)
{
    DEBUG_PRINT_LOW("venc_set_vpe_rotation: rotation angle = %d", (int)rotation_angle);
 struct v4l2_control control;
 int rc;
 struct v4l2_format fmt;
 struct v4l2_requestbuffers bufreq;

    control.id = V4L2_CID_MPEG_VIDC_VIDEO_ROTATION;
 if (rotation_angle == 0)
        control.value = V4L2_CID_MPEG_VIDC_VIDEO_ROTATION_NONE;
 else if (rotation_angle == 90)
        control.value = V4L2_CID_MPEG_VIDC_VIDEO_ROTATION_90;
 else if (rotation_angle == 180)
        control.value = V4L2_CID_MPEG_VIDC_VIDEO_ROTATION_180;
 else if (rotation_angle == 270)
        control.value = V4L2_CID_MPEG_VIDC_VIDEO_ROTATION_270;
 else {
        DEBUG_PRINT_ERROR("Failed to find valid rotation angle");
 return false;
 }

    DEBUG_PRINT_LOW("Calling IOCTL set control for id=%x, val=%d", control.id, control.value);
    rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);
 if (rc) {
        DEBUG_PRINT_HIGH("Failed to set VPE Rotation control");
 return false;
 }
    DEBUG_PRINT_LOW("Success IOCTL set control for id=%x, value=%d", control.id, control.value);

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    fmt.fmt.pix_mp.height = m_sVenc_cfg.dvs_height;
    fmt.fmt.pix_mp.width = m_sVenc_cfg.dvs_width;
    fmt.fmt.pix_mp.pixelformat = m_sVenc_cfg.codectype;
 if (ioctl(m_nDriver_fd, VIDIOC_S_FMT, &fmt)) {
        DEBUG_PRINT_ERROR("Failed to set format on capture port");
 return false;
 }

    m_sOutput_buff_property.datasize = fmt.fmt.pix_mp.plane_fmt[0].sizeimage;
    bufreq.memory = V4L2_MEMORY_USERPTR;
    bufreq.count = m_sOutput_buff_property.actualcount;
    bufreq.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
 if (ioctl(m_nDriver_fd,VIDIOC_REQBUFS, &bufreq)) {
        DEBUG_PRINT_ERROR("ERROR: Request for o/p buffer count failed for rotation");
 return false;
 }
 if (bufreq.count >= m_sOutput_buff_property.mincount)
        m_sOutput_buff_property.actualcount = m_sOutput_buff_property.mincount = bufreq.count;

 return true;
}
