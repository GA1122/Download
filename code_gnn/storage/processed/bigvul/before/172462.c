bool venc_dev::venc_set_idr_period(OMX_U32 nPFrames, OMX_U32 nIDRPeriod)
{
 int rc = 0;
 struct v4l2_control control;
    DEBUG_PRINT_LOW("venc_set_idr_period: nPFrames = %u, nIDRPeriod: %u",
 (unsigned int)nPFrames, (unsigned int)nIDRPeriod);

 if (m_sVenc_cfg.codectype != V4L2_PIX_FMT_H264) {
        DEBUG_PRINT_ERROR("ERROR: IDR period valid for H264 only!!");
 return false;
 }

 if (venc_set_intra_period (nPFrames, intra_period.num_bframes) == false) {
        DEBUG_PRINT_ERROR("ERROR: Request for setting intra period failed");
 return false;
 }

 if (!intra_period.num_bframes)
    intra_period.num_pframes = nPFrames;
    control.id = V4L2_CID_MPEG_VIDC_VIDEO_IDR_PERIOD;
    control.value = nIDRPeriod;

    rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);

 if (rc) {
        DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }

    idrperiod.idrperiod = nIDRPeriod;
 return true;
}
