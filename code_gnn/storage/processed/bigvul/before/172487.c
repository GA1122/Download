unsigned venc_dev::venc_start(void)
{
 enum v4l2_buf_type buf_type;
 int ret, r;
 struct v4l2_control control;

    memset(&control, 0, sizeof(control));

    DEBUG_PRINT_HIGH("%s(): Check Profile/Level set in driver before start",
            __func__);
    m_level_set = false;

 if (!venc_set_profile_level(0, 0)) {
        DEBUG_PRINT_ERROR("ERROR: %s(): Driver Profile/Level is NOT SET",
                __func__);
 } else {
        DEBUG_PRINT_HIGH("%s(): Driver Profile[%lu]/Level[%lu] successfully SET",
                __func__, codec_profile.profile, profile_level.level);
 }

    venc_config_print();

 if(resume_in_stopped){
  
        venc_reconfig_reqbufs();
        resume_in_stopped = 0;
 }

  
 if (slice_mode.enable && multislice.mslice_size &&
 (m_sVenc_cfg.dvs_width *  m_sVenc_cfg.dvs_height)/(256 * multislice.mslice_size) >= MAX_SUPPORTED_SLICES_PER_FRAME) {
        DEBUG_PRINT_ERROR("slice_mode: %lu, max slices (%lu) should be less than (%d)", slice_mode.enable,
 (m_sVenc_cfg.dvs_width *  m_sVenc_cfg.dvs_height)/(256 * multislice.mslice_size),
                MAX_SUPPORTED_SLICES_PER_FRAME);
 return 1;
 }

    buf_type=V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    DEBUG_PRINT_LOW("send_command_proxy(): Idle-->Executing");
    ret=ioctl(m_nDriver_fd, VIDIOC_STREAMON,&buf_type);

 if (ret)
 return 1;

    streaming[CAPTURE_PORT] = true;

    control.id = V4L2_CID_MPEG_VIDC_VIDEO_REQUEST_SEQ_HEADER;
    control.value = 1;
    ret = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);
 if (ret) {
        DEBUG_PRINT_ERROR("failed to request seq header");
 return 1;
 }

    stopped = 0;
 return 0;
}
