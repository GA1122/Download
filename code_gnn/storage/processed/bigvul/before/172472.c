bool venc_dev::venc_set_operatingrate(OMX_U32 rate) {
 struct v4l2_control control;

    control.id = V4L2_CID_MPEG_VIDC_VIDEO_OPERATING_RATE;
    control.value = rate;

    DEBUG_PRINT_LOW("venc_set_operating_rate: %d fps", rate >> 16);
    DEBUG_PRINT_LOW("Calling IOCTL set control for id=%d, val=%d", control.id, control.value);

 if(ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control)) {
        hw_overload = errno == EBUSY;
        DEBUG_PRINT_ERROR("Failed to set operating rate %d fps (%s)",
                rate >> 16, hw_overload ? "HW overload" : strerror(errno));
 return false;
 }
    operating_rate = rate;
    DEBUG_PRINT_LOW("Operating Rate Set = %d fps",  rate >> 16);
 return true;
}
