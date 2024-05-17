bool venc_dev::venc_set_intra_refresh(OMX_VIDEO_INTRAREFRESHTYPE ir_mode, OMX_U32 irMBs)
{
 bool status = true;
 int rc;
 struct v4l2_control control_mode,control_mbs;
    control_mode.id = V4L2_CID_MPEG_VIDC_VIDEO_INTRA_REFRESH_MODE;

 if (irMBs == 0 || ir_mode == OMX_VIDEO_IntraRefreshMax) {
        control_mode.value = V4L2_CID_MPEG_VIDC_VIDEO_INTRA_REFRESH_NONE;
 return status;
 } else if ((ir_mode == OMX_VIDEO_IntraRefreshCyclic) &&
 (irMBs < ((m_sVenc_cfg.dvs_width * m_sVenc_cfg.dvs_height)>>8))) {
        control_mode.value = V4L2_CID_MPEG_VIDC_VIDEO_INTRA_REFRESH_CYCLIC;
        control_mbs.id=V4L2_CID_MPEG_VIDC_VIDEO_CIR_MBS;
        control_mbs.value=irMBs;
 } else if ((ir_mode == OMX_VIDEO_IntraRefreshAdaptive) &&
 (irMBs < ((m_sVenc_cfg.dvs_width * m_sVenc_cfg.dvs_height)>>8))) {
        control_mode.value = V4L2_CID_MPEG_VIDC_VIDEO_INTRA_REFRESH_ADAPTIVE;
        control_mbs.id=V4L2_CID_MPEG_VIDC_VIDEO_AIR_MBS;
        control_mbs.value=irMBs;
 } else if ((ir_mode == OMX_VIDEO_IntraRefreshBoth) &&
 (irMBs < ((m_sVenc_cfg.dvs_width * m_sVenc_cfg.dvs_height)>>8))) {
        control_mode.value = V4L2_CID_MPEG_VIDC_VIDEO_INTRA_REFRESH_CYCLIC_ADAPTIVE;
 } else if ((ir_mode == OMX_VIDEO_IntraRefreshRandom) &&
 (irMBs < ((m_sVenc_cfg.dvs_width * m_sVenc_cfg.dvs_height)>>8))) {
        control_mode.value = V4L2_CID_MPEG_VIDC_VIDEO_INTRA_REFRESH_RANDOM;
        control_mbs.id = V4L2_CID_MPEG_VIDC_VIDEO_AIR_MBS;
        control_mbs.value = irMBs;
 } else {
        DEBUG_PRINT_ERROR("ERROR: Invalid IntraRefresh Parameters:"
 "mb count: %u, mb mode:%d", (unsigned int)irMBs, ir_mode);
 return false;
 }

    DEBUG_PRINT_LOW("Calling IOCTL set control for id=%u, val=%d", control_mode.id, control_mode.value);
    rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control_mode);

 if (rc) {
        DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }

    DEBUG_PRINT_LOW("Success IOCTL set control for id=%d, value=%d", control_mode.id, control_mode.value);

    DEBUG_PRINT_LOW("Calling IOCTL set control for id=%d, val=%d", control_mbs.id, control_mbs.value);
    rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control_mbs);

 if (rc) {
        DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }

    DEBUG_PRINT_LOW("Success IOCTL set control for id=%d, value=%d", control_mbs.id, control_mbs.value);

    intra_refresh.irmode = control_mode.value;
    intra_refresh.mbcount = control_mbs.value;

 return status;
}
