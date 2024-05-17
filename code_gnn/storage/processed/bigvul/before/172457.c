bool venc_dev::venc_set_error_resilience(OMX_VIDEO_PARAM_ERRORCORRECTIONTYPE* error_resilience)
{
 bool status = true;
 struct venc_headerextension hec_cfg;
 struct venc_multiclicecfg multislice_cfg;
 int rc;
    OMX_U32 resynchMarkerSpacingBytes = 0;
 struct v4l2_control control;

    memset(&control, 0, sizeof(control));

 if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_MPEG4) {
 if (error_resilience->bEnableHEC) {
            hec_cfg.header_extension = 1;
 } else {
            hec_cfg.header_extension = 0;
 }

        hec.header_extension = error_resilience->bEnableHEC;
 }

 if (error_resilience->bEnableRVLC) {
        DEBUG_PRINT_ERROR("RVLC is not Supported");
 return false;
 }

 if (( m_sVenc_cfg.codectype != V4L2_PIX_FMT_H263) &&
 (error_resilience->bEnableDataPartitioning)) {
        DEBUG_PRINT_ERROR("DataPartioning are not Supported for MPEG4/H264");
 return false;
 }

 if (error_resilience->nResynchMarkerSpacing) {
        resynchMarkerSpacingBytes = error_resilience->nResynchMarkerSpacing;
        resynchMarkerSpacingBytes = ALIGN(resynchMarkerSpacingBytes, 8) >> 3;
 }
 if (( m_sVenc_cfg.codectype != V4L2_PIX_FMT_H263) &&
 (error_resilience->nResynchMarkerSpacing)) {
        multislice_cfg.mslice_mode = VEN_MSLICE_CNT_BYTE;
        multislice_cfg.mslice_size = resynchMarkerSpacingBytes;
        control.id = V4L2_CID_MPEG_VIDEO_MULTI_SLICE_MODE;
        control.value = V4L2_MPEG_VIDEO_MULTI_SICE_MODE_MAX_BYTES;
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_H263 &&
            error_resilience->bEnableDataPartitioning) {
        multislice_cfg.mslice_mode = VEN_MSLICE_GOB;
        multislice_cfg.mslice_size = resynchMarkerSpacingBytes;
        control.id = V4L2_CID_MPEG_VIDEO_MULTI_SLICE_MODE;
        control.value = V4L2_MPEG_VIDEO_MULTI_SLICE_GOB;
 } else {
        multislice_cfg.mslice_mode = VEN_MSLICE_OFF;
        multislice_cfg.mslice_size = 0;
        control.id = V4L2_CID_MPEG_VIDEO_MULTI_SLICE_MODE;
        control.value =  V4L2_MPEG_VIDEO_MULTI_SLICE_MODE_SINGLE;
 }

    DEBUG_PRINT_LOW("%s(): mode = %lu, size = %lu", __func__,
            multislice_cfg.mslice_mode, multislice_cfg.mslice_size);
    DEBUG_PRINT_ERROR("Calling IOCTL set control for id=%x, val=%d", control.id, control.value);
    rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);

 if (rc) {
       DEBUG_PRINT_ERROR("Failed to set Slice mode control");
 return false;
 }

    DEBUG_PRINT_ERROR("Success IOCTL set control for id=%x, value=%d", control.id, control.value);
    multislice.mslice_mode=control.value;

    control.id = V4L2_CID_MPEG_VIDEO_MULTI_SLICE_MAX_BYTES;
    control.value = resynchMarkerSpacingBytes;
    DEBUG_PRINT_ERROR("Calling IOCTL set control for id=%x, val=%d", control.id, control.value);

    rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);

 if (rc) {
       DEBUG_PRINT_ERROR("Failed to set MAX MB control");
 return false;
 }

    DEBUG_PRINT_ERROR("Success IOCTL set control for id=%x, value=%d", control.id, control.value);
    multislice.mslice_mode = multislice_cfg.mslice_mode;
    multislice.mslice_size = multislice_cfg.mslice_size;
 return status;
}
