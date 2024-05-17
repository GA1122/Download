bool venc_dev::venc_set_deinterlace(OMX_U32 enable)
{
    DEBUG_PRINT_LOW("venc_set_deinterlace: enable = %u", (unsigned int)enable);
 struct v4l2_control control;
 int rc;
    control.id = V4L2_CID_MPEG_VIDC_VIDEO_DEINTERLACE;
 if (enable)
        control.value = V4L2_CID_MPEG_VIDC_VIDEO_DEINTERLACE_ENABLED;
 else
        control.value = V4L2_CID_MPEG_VIDC_VIDEO_DEINTERLACE_ENABLED;

    DEBUG_PRINT_LOW("Calling IOCTL set control for id=%x, val=%d", control.id, control.value);
    rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);
 if (rc) {
        DEBUG_PRINT_ERROR("Failed to set Deinterlcing control");
 return false;
 }
    DEBUG_PRINT_LOW("Success IOCTL set control for id=%x, value=%d", control.id, control.value);
    deinterlace_enabled = true;
 return true;
}
