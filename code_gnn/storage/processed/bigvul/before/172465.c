bool venc_dev::venc_set_intra_period(OMX_U32 nPFrames, OMX_U32 nBFrames)
{

    DEBUG_PRINT_LOW("venc_set_intra_period: nPFrames = %u, nBFrames: %u", (unsigned int)nPFrames, (unsigned int)nBFrames);
 int rc;
 struct v4l2_control control;
 int pframe = 0, bframe = 0;

 if ((codec_profile.profile != V4L2_MPEG_VIDEO_MPEG4_PROFILE_ADVANCED_SIMPLE) &&
 (codec_profile.profile != V4L2_MPEG_VIDEO_H264_PROFILE_MAIN) &&
 (codec_profile.profile != V4L2_MPEG_VIDC_VIDEO_HEVC_PROFILE_MAIN) &&
 (codec_profile.profile != V4L2_MPEG_VIDC_VIDEO_HEVC_PROFILE_MAIN10) &&
 (codec_profile.profile != V4L2_MPEG_VIDEO_H264_PROFILE_HIGH)) {
        nBFrames=0;
 }

 if (!venc_validate_hybridhp_params(0, nBFrames, 0, 0)) {
        DEBUG_PRINT_ERROR("Invalid settings, bframes cannot be enabled with HybridHP");
 return false;
 }

    intra_period.num_pframes = nPFrames;
    intra_period.num_bframes = nBFrames;

 if (!venc_calibrate_gop())
 {
        DEBUG_PRINT_ERROR("Invalid settings, Hybrid HP enabled with LTR OR Hier-pLayers OR bframes");
 return false;
 }

    control.id = V4L2_CID_MPEG_VIDC_VIDEO_NUM_P_FRAMES;
    control.value = intra_period.num_pframes;
    rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);

 if (rc) {
        DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }

    DEBUG_PRINT_LOW("Success IOCTL set control for id=%d, value=%d", control.id, control.value);

    control.id = V4L2_CID_MPEG_VIDC_VIDEO_NUM_B_FRAMES;
    control.value = intra_period.num_bframes;
    DEBUG_PRINT_LOW("Calling IOCTL set control for id=%d, val=%d", control.id, control.value);
    rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);

 if (rc) {
        DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }

    DEBUG_PRINT_LOW("Success IOCTL set control for id=%d, value=%lu", control.id, intra_period.num_bframes);

 if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_H264) {
        control.id = V4L2_CID_MPEG_VIDC_VIDEO_IDR_PERIOD;
        control.value = 1;

        rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);

 if (rc) {
            DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }

        idrperiod.idrperiod = 1;
 }

 return true;
}
