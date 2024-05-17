bool venc_dev::venc_open(OMX_U32 codec)
{
 int r;
 unsigned int alignment = 0,buffer_size = 0, temp =0;
 struct v4l2_control control;
    OMX_STRING device_name = (OMX_STRING)"/dev/video33";
 char property_value[PROPERTY_VALUE_MAX] = {0};
 char platform_name[PROPERTY_VALUE_MAX] = {0};

    property_get("ro.board.platform", platform_name, "0");
    property_get("vidc.enc.narrow.searchrange", property_value, "0");
    enable_mv_narrow_searchrange = atoi(property_value);

 if (!strncmp(platform_name, "msm8610", 7)) {
        device_name = (OMX_STRING)"/dev/video/q6_enc";
        supported_rc_modes = (RC_ALL & ~RC_CBR_CFR);
 }
    m_nDriver_fd = open (device_name, O_RDWR);

 if (m_nDriver_fd == 0) {
        DEBUG_PRINT_ERROR("ERROR: Got fd as 0 for msm_vidc_enc, Opening again");
        m_nDriver_fd = open (device_name, O_RDWR);
 }

 if ((int)m_nDriver_fd < 0) {
        DEBUG_PRINT_ERROR("ERROR: Omx_venc::Comp Init Returning failure");
 return false;
 }

    DEBUG_PRINT_LOW("m_nDriver_fd = %u", (unsigned int)m_nDriver_fd);
    m_sVenc_cfg.input_width = OMX_CORE_QCIF_WIDTH;
    m_sVenc_cfg.input_height= OMX_CORE_QCIF_HEIGHT;
    m_sVenc_cfg.dvs_width = OMX_CORE_QCIF_WIDTH;
    m_sVenc_cfg.dvs_height = OMX_CORE_QCIF_HEIGHT;
    m_sVenc_cfg.fps_num = 30;
    m_sVenc_cfg.fps_den = 1;
    m_sVenc_cfg.targetbitrate = 64000;
    m_sVenc_cfg.inputformat= V4L2_PIX_FMT_NV12;
    m_codec = codec;

 if (codec == OMX_VIDEO_CodingMPEG4) {
        m_sVenc_cfg.codectype = V4L2_PIX_FMT_MPEG4;
        codec_profile.profile = V4L2_MPEG_VIDEO_MPEG4_PROFILE_SIMPLE;
        profile_level.level = V4L2_MPEG_VIDEO_MPEG4_LEVEL_2;
        session_qp_range.minqp = 1;
        session_qp_range.maxqp = 31;
 } else if (codec == OMX_VIDEO_CodingH263) {
        m_sVenc_cfg.codectype = V4L2_PIX_FMT_H263;
        codec_profile.profile = VEN_PROFILE_H263_BASELINE;
        profile_level.level = VEN_LEVEL_H263_20;
        session_qp_range.minqp = 1;
        session_qp_range.maxqp = 31;
 } else if (codec == OMX_VIDEO_CodingAVC) {
        m_sVenc_cfg.codectype = V4L2_PIX_FMT_H264;
        codec_profile.profile = V4L2_MPEG_VIDEO_H264_PROFILE_BASELINE;
        profile_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_1_0;
        session_qp_range.minqp = 1;
        session_qp_range.maxqp = 51;
 } else if (codec == OMX_VIDEO_CodingVP8) {
        m_sVenc_cfg.codectype = V4L2_PIX_FMT_VP8;
        codec_profile.profile = V4L2_MPEG_VIDC_VIDEO_VP8_UNUSED;
        profile_level.level = V4L2_MPEG_VIDC_VIDEO_VP8_VERSION_0;
        session_qp_range.minqp = 1;
        session_qp_range.maxqp = 128;
 } else if (codec == OMX_VIDEO_CodingHEVC) {
        m_sVenc_cfg.codectype = V4L2_PIX_FMT_HEVC;
        session_qp_range.minqp = 1;
        session_qp_range.maxqp = 51;
        codec_profile.profile = V4L2_MPEG_VIDC_VIDEO_HEVC_PROFILE_MAIN;
        profile_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_1;
 }
    session_qp_values.minqp = session_qp_range.minqp;
    session_qp_values.maxqp = session_qp_range.maxqp;

 int ret;
    ret = subscribe_to_events(m_nDriver_fd);

 if (ret) {
        DEBUG_PRINT_ERROR("Subscribe Event Failed");
 return false;
 }

 struct v4l2_capability cap;

 struct v4l2_fmtdesc fdesc;

 struct v4l2_format fmt;

 struct v4l2_requestbuffers bufreq;

    ret = ioctl(m_nDriver_fd, VIDIOC_QUERYCAP, &cap);

 if (ret) {
        DEBUG_PRINT_ERROR("Failed to query capabilities");
 } else {
        DEBUG_PRINT_LOW("Capabilities: driver_name = %s, card = %s, bus_info = %s,"
 " version = %d, capabilities = %x", cap.driver, cap.card,
                cap.bus_info, cap.version, cap.capabilities);
 }

    ret=0;
    fdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    fdesc.index=0;

 while (ioctl(m_nDriver_fd, VIDIOC_ENUM_FMT, &fdesc) == 0) {
        DEBUG_PRINT_LOW("fmt: description: %s, fmt: %x, flags = %x", fdesc.description,
                fdesc.pixelformat, fdesc.flags);
        fdesc.index++;
 }

    fdesc.type=V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    fdesc.index=0;

 while (ioctl(m_nDriver_fd, VIDIOC_ENUM_FMT, &fdesc) == 0) {
        DEBUG_PRINT_LOW("fmt: description: %s, fmt: %x, flags = %x", fdesc.description,
                fdesc.pixelformat, fdesc.flags);
        fdesc.index++;
 }

 if (venc_handle->is_secure_session()) {
        m_sOutput_buff_property.alignment = SZ_1M;
        m_sInput_buff_property.alignment  = SZ_1M;
 } else {
        m_sOutput_buff_property.alignment = SZ_4K;
        m_sInput_buff_property.alignment  = SZ_4K;
 }
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    fmt.fmt.pix_mp.height = m_sVenc_cfg.dvs_height;
    fmt.fmt.pix_mp.width = m_sVenc_cfg.dvs_width;
    fmt.fmt.pix_mp.pixelformat = m_sVenc_cfg.codectype;

  
    ret = ioctl(m_nDriver_fd, VIDIOC_S_FMT, &fmt);

 if (ret) {
        DEBUG_PRINT_ERROR("Failed to set format on capture port");
 return false;
 }

    m_sOutput_buff_property.datasize=fmt.fmt.pix_mp.plane_fmt[0].sizeimage;

    fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    fmt.fmt.pix_mp.height = m_sVenc_cfg.input_height;
    fmt.fmt.pix_mp.width = m_sVenc_cfg.input_width;
    fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_NV12;
    fmt.fmt.pix_mp.colorspace = V4L2_COLORSPACE_BT878;

    ret = ioctl(m_nDriver_fd, VIDIOC_S_FMT, &fmt);
    m_sInput_buff_property.datasize=fmt.fmt.pix_mp.plane_fmt[0].sizeimage;

    bufreq.memory = V4L2_MEMORY_USERPTR;
    bufreq.count = 2;

    bufreq.type=V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    ret = ioctl(m_nDriver_fd,VIDIOC_REQBUFS, &bufreq);
    m_sInput_buff_property.mincount = m_sInput_buff_property.actualcount = bufreq.count;

    bufreq.type=V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    bufreq.count = 2;
    ret = ioctl(m_nDriver_fd,VIDIOC_REQBUFS, &bufreq);
    m_sOutput_buff_property.mincount = m_sOutput_buff_property.actualcount = bufreq.count;

 if(venc_handle->is_secure_session()) {
        control.id = V4L2_CID_MPEG_VIDC_VIDEO_SECURE;
        control.value = 1;
        DEBUG_PRINT_HIGH("ioctl: open secure device");
        ret=ioctl(m_nDriver_fd, VIDIOC_S_CTRL,&control);
 if (ret) {
            DEBUG_PRINT_ERROR("ioctl: open secure dev fail, rc %d", ret);
 return false;
 }
 }

    resume_in_stopped = 0;
    metadatamode = 0;
    camera_mode_enabled = false;

    control.id = V4L2_CID_MPEG_VIDEO_HEADER_MODE;
    control.value = V4L2_MPEG_VIDEO_HEADER_MODE_SEPARATE;

    DEBUG_PRINT_LOW("Calling IOCTL to disable seq_hdr in sync_frame id=%d, val=%d", control.id, control.value);

 if (ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control))
        DEBUG_PRINT_ERROR("Failed to set control");

 struct v4l2_frmsizeenum frmsize;

    memset((void *)&frmsize,0,sizeof(frmsize));
    frmsize.index = 0;
    frmsize.pixel_format = m_sVenc_cfg.codectype;
    ret = ioctl(m_nDriver_fd, VIDIOC_ENUM_FRAMESIZES, &frmsize);

 if (ret || frmsize.type != V4L2_FRMSIZE_TYPE_STEPWISE) {
        DEBUG_PRINT_ERROR("Failed to get framesizes");
 return false;
 }

 if (frmsize.type == V4L2_FRMSIZE_TYPE_STEPWISE) {
        capability.min_width = frmsize.stepwise.min_width;
        capability.max_width = frmsize.stepwise.max_width;
        capability.min_height = frmsize.stepwise.min_height;
        capability.max_height = frmsize.stepwise.max_height;
 }
 if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_VP8) {
        control.id = V4L2_CID_MPEG_VIDC_VIDEO_NUM_P_FRAMES;
        control.value = 0x7fffffff;
 if (ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control))
            DEBUG_PRINT_ERROR("Failed to set V4L2_CID_MPEG_VIDC_VIDEO_NUM_P_FRAME\n");
 }

    property_get("vidc.debug.turbo", property_value, "0");
 if (atoi(property_value)) {
        DEBUG_PRINT_HIGH("Turbo mode debug property enabled");
        control.id = V4L2_CID_MPEG_VIDC_SET_PERF_LEVEL;
        control.value = V4L2_CID_MPEG_VIDC_PERF_LEVEL_TURBO;
 if (ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control)) {
            DEBUG_PRINT_ERROR("Failed to set turbo mode");
 }
 }

    sess_priority.priority = 1;  
 if (venc_set_session_priority(sess_priority.priority)) {
        DEBUG_PRINT_ERROR("Setting session priority failed");
 return OMX_ErrorUnsupportedSetting;
 }
 return true;
}
