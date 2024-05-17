bool venc_dev::venc_set_slice_delivery_mode(OMX_U32 enable)
{
 struct v4l2_control control;

 if (enable) {
        control.id = V4L2_CID_MPEG_VIDEO_MULTI_SLICE_DELIVERY_MODE;
        control.value = 1;
        DEBUG_PRINT_LOW("Set slice_delivery_mode: %d", control.value);

 if (multislice.mslice_mode == V4L2_MPEG_VIDEO_MULTI_SICE_MODE_MAX_MB && m_sVenc_cfg.codectype == V4L2_PIX_FMT_H264) {
 if (ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control)) {
                DEBUG_PRINT_ERROR("Request for setting slice delivery mode failed");
 return false;
 } else {
                DEBUG_PRINT_LOW("Successfully set Slice delivery mode id: %d, value=%d", control.id, control.value);
                slice_mode.enable = 1;
 }
 } else {
            DEBUG_PRINT_ERROR("Failed to set slice delivery mode, slice_mode [%lu] "
 "is not MB BASED or [%lu] is not H264 codec ", multislice.mslice_mode,
                    m_sVenc_cfg.codectype);
 }
 } else {
        DEBUG_PRINT_ERROR("Slice_DELIVERY_MODE not enabled");
 }

 return true;
}
