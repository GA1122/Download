OMX_ERRORTYPE omx_vdec::component_init(OMX_STRING role)
{

    OMX_ERRORTYPE eRet = OMX_ErrorNone;
 struct v4l2_fmtdesc fdesc;
 struct v4l2_format fmt;
 struct v4l2_requestbuffers bufreq;
 struct v4l2_control control;
 struct v4l2_frmsizeenum frmsize;
 unsigned int   alignment = 0,buffer_size = 0;
 int fds[2];
 int r,ret=0;
 bool codec_ambiguous = false;
    OMX_STRING device_name = (OMX_STRING)"/dev/video32";
 char property_value[PROPERTY_VALUE_MAX] = {0};

#ifdef _ANDROID_
 char platform_name[PROPERTY_VALUE_MAX];
    property_get("ro.board.platform", platform_name, "0");
 if (!strncmp(platform_name, "msm8610", 7)) {
        device_name = (OMX_STRING)"/dev/video/q6_dec";
        is_q6_platform = true;
        maxSmoothStreamingWidth = 1280;
        maxSmoothStreamingHeight = 720;
 }
#endif

#ifdef _ANDROID_
  
    arbitrary_bytes = false;
    property_get("vidc.dec.debug.arbitrarybytes.mode", property_value, "0");
 if (atoi(property_value)) {
        DEBUG_PRINT_HIGH("arbitrary_bytes mode enabled via property command");
        arbitrary_bytes = true;
 }
#endif

 if (!strncmp(role, "OMX.qcom.video.decoder.avc.secure",
                OMX_MAX_STRINGNAME_SIZE)) {
        secure_mode = true;
        arbitrary_bytes = false;
        role = (OMX_STRING)"OMX.qcom.video.decoder.avc";
 } else if (!strncmp(role, "OMX.qcom.video.decoder.mpeg2.secure",
                OMX_MAX_STRINGNAME_SIZE)) {
        secure_mode = true;
        arbitrary_bytes = false;
        role = (OMX_STRING)"OMX.qcom.video.decoder.mpeg2";
 } else if (!strncmp(role, "OMX.qcom.video.decoder.hevc.secure",
                OMX_MAX_STRINGNAME_SIZE)) {
        secure_mode = true;
        arbitrary_bytes = false;
        role = (OMX_STRING)"OMX.qcom.video.decoder.hevc";
 } else if (!strncmp(role, "OMX.qcom.video.decoder.vc1.secure",
                OMX_MAX_STRINGNAME_SIZE)) {
        secure_mode = true;
        arbitrary_bytes = false;
        role = (OMX_STRING)"OMX.qcom.video.decoder.vc1";
 } else if (!strncmp(role, "OMX.qcom.video.decoder.wmv.secure",
                OMX_MAX_STRINGNAME_SIZE)) {
        secure_mode = true;
        arbitrary_bytes = false;
        role = (OMX_STRING)"OMX.qcom.video.decoder.wmv";
 } else if (!strncmp(role, "OMX.qcom.video.decoder.mpeg4.secure",
                OMX_MAX_STRINGNAME_SIZE)) {
        secure_mode = true;
        arbitrary_bytes = false;
        role = (OMX_STRING)"OMX.qcom.video.decoder.mpeg4";
 }

    drv_ctx.video_driver_fd = open(device_name, O_RDWR);

    DEBUG_PRINT_INFO("component_init: %s : fd=%d", role, drv_ctx.video_driver_fd);

 if (drv_ctx.video_driver_fd == 0) {
        DEBUG_PRINT_ERROR("omx_vdec_msm8974 :: Got fd as 0 for msm_vidc_dec, Opening again");
        drv_ctx.video_driver_fd = open(device_name, O_RDWR);
        close(0);
 }

 if (drv_ctx.video_driver_fd < 0) {
        DEBUG_PRINT_ERROR("Omx_vdec::Comp Init Returning failure, errno %d", errno);
 return OMX_ErrorInsufficientResources;
 }
    drv_ctx.frame_rate.fps_numerator = DEFAULT_FPS;
    drv_ctx.frame_rate.fps_denominator = 1;

    ret = subscribe_to_events(drv_ctx.video_driver_fd);
 if (!ret) {
        async_thread_created = true;
        ret = pthread_create(&async_thread_id,0,async_message_thread,this);
 }
 if (ret) {
        DEBUG_PRINT_ERROR("Failed to create async_message_thread");
        async_thread_created = false;
 return OMX_ErrorInsufficientResources;
 }

#ifdef OUTPUT_EXTRADATA_LOG
    outputExtradataFile = fopen (output_extradata_filename, "ab");
#endif

    strlcpy(drv_ctx.kind,role,128);

 if (!strncmp(drv_ctx.kind,"OMX.qcom.video.decoder.mpeg4",\
                OMX_MAX_STRINGNAME_SIZE)) {
        strlcpy((char *)m_cRole, "video_decoder.mpeg4",\
                OMX_MAX_STRINGNAME_SIZE);
        drv_ctx.timestamp_adjust = true;
        drv_ctx.decoder_format = VDEC_CODECTYPE_MPEG4;
        eCompressionFormat = OMX_VIDEO_CodingMPEG4;
        output_capability=V4L2_PIX_FMT_MPEG4;
  
        codec_type_parse = CODEC_TYPE_MPEG4;
        m_frame_parser.init_start_codes (codec_type_parse);
 } else if (!strncmp(drv_ctx.kind,"OMX.qcom.video.decoder.mpeg2",\
                OMX_MAX_STRINGNAME_SIZE)) {
        strlcpy((char *)m_cRole, "video_decoder.mpeg2",\
                OMX_MAX_STRINGNAME_SIZE);
        drv_ctx.decoder_format = VDEC_CODECTYPE_MPEG2;
        output_capability = V4L2_PIX_FMT_MPEG2;
        eCompressionFormat = OMX_VIDEO_CodingMPEG2;
  
        codec_type_parse = CODEC_TYPE_MPEG2;
        m_frame_parser.init_start_codes (codec_type_parse);
 } else if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.h263",\
                OMX_MAX_STRINGNAME_SIZE)) {
        strlcpy((char *)m_cRole, "video_decoder.h263",OMX_MAX_STRINGNAME_SIZE);
        DEBUG_PRINT_LOW("H263 Decoder selected");
        drv_ctx.decoder_format = VDEC_CODECTYPE_H263;
        eCompressionFormat = OMX_VIDEO_CodingH263;
        output_capability = V4L2_PIX_FMT_H263;
        codec_type_parse = CODEC_TYPE_H263;
        m_frame_parser.init_start_codes (codec_type_parse);
 } else if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.divx311",\
                OMX_MAX_STRINGNAME_SIZE)) {
        strlcpy((char *)m_cRole, "video_decoder.divx",OMX_MAX_STRINGNAME_SIZE);
        DEBUG_PRINT_LOW ("DIVX 311 Decoder selected");
        drv_ctx.decoder_format = VDEC_CODECTYPE_DIVX_3;
        output_capability = V4L2_PIX_FMT_DIVX_311;
        eCompressionFormat = (OMX_VIDEO_CODINGTYPE)QOMX_VIDEO_CodingDivx;
        codec_type_parse = CODEC_TYPE_DIVX;
        m_frame_parser.init_start_codes (codec_type_parse);

        eRet = createDivxDrmContext();
 if (eRet != OMX_ErrorNone) {
            DEBUG_PRINT_ERROR("createDivxDrmContext Failed");
 return eRet;
 }
 } else if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.divx4",\
                OMX_MAX_STRINGNAME_SIZE)) {
        strlcpy((char *)m_cRole, "video_decoder.divx",OMX_MAX_STRINGNAME_SIZE);
        DEBUG_PRINT_ERROR ("DIVX 4 Decoder selected");
        drv_ctx.decoder_format = VDEC_CODECTYPE_DIVX_4;
        output_capability = V4L2_PIX_FMT_DIVX;
        eCompressionFormat = (OMX_VIDEO_CODINGTYPE)QOMX_VIDEO_CodingDivx;
        codec_type_parse = CODEC_TYPE_DIVX;
        codec_ambiguous = true;
        m_frame_parser.init_start_codes (codec_type_parse);

        eRet = createDivxDrmContext();
 if (eRet != OMX_ErrorNone) {
            DEBUG_PRINT_ERROR("createDivxDrmContext Failed");
 return eRet;
 }
 } else if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.divx",\
                OMX_MAX_STRINGNAME_SIZE)) {
        strlcpy((char *)m_cRole, "video_decoder.divx",OMX_MAX_STRINGNAME_SIZE);
        DEBUG_PRINT_ERROR ("DIVX 5/6 Decoder selected");
        drv_ctx.decoder_format = VDEC_CODECTYPE_DIVX_6;
        output_capability = V4L2_PIX_FMT_DIVX;
        eCompressionFormat = (OMX_VIDEO_CODINGTYPE)QOMX_VIDEO_CodingDivx;
        codec_type_parse = CODEC_TYPE_DIVX;
        codec_ambiguous = true;
        m_frame_parser.init_start_codes (codec_type_parse);

        eRet = createDivxDrmContext();
 if (eRet != OMX_ErrorNone) {
            DEBUG_PRINT_ERROR("createDivxDrmContext Failed");
 return eRet;
 }

 } else if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.avc",\
                OMX_MAX_STRINGNAME_SIZE)) {
        strlcpy((char *)m_cRole, "video_decoder.avc",OMX_MAX_STRINGNAME_SIZE);
        drv_ctx.decoder_format = VDEC_CODECTYPE_H264;
        output_capability=V4L2_PIX_FMT_H264;
        eCompressionFormat = OMX_VIDEO_CodingAVC;
        codec_type_parse = CODEC_TYPE_H264;
        m_frame_parser.init_start_codes (codec_type_parse);
        m_frame_parser.init_nal_length(nal_length);
 } else if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.mvc",\
                OMX_MAX_STRINGNAME_SIZE)) {
        strlcpy((char *)m_cRole, "video_decoder.mvc", OMX_MAX_STRINGNAME_SIZE);
        drv_ctx.decoder_format = VDEC_CODECTYPE_MVC;
        output_capability = V4L2_PIX_FMT_H264_MVC;
        eCompressionFormat = (OMX_VIDEO_CODINGTYPE)QOMX_VIDEO_CodingMVC;
        codec_type_parse = CODEC_TYPE_H264;
        m_frame_parser.init_start_codes(codec_type_parse);
        m_frame_parser.init_nal_length(nal_length);
 } else if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.hevc",\
                OMX_MAX_STRINGNAME_SIZE)) {
        strlcpy((char *)m_cRole, "video_decoder.hevc",OMX_MAX_STRINGNAME_SIZE);
        drv_ctx.decoder_format = VDEC_CODECTYPE_HEVC;
        output_capability = V4L2_PIX_FMT_HEVC;
        eCompressionFormat = (OMX_VIDEO_CODINGTYPE)QOMX_VIDEO_CodingHevc;
        codec_type_parse = CODEC_TYPE_HEVC;
        m_frame_parser.init_start_codes(codec_type_parse);
        m_frame_parser.init_nal_length(nal_length);
 } else if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.vc1",\
                OMX_MAX_STRINGNAME_SIZE)) {
        strlcpy((char *)m_cRole, "video_decoder.vc1",OMX_MAX_STRINGNAME_SIZE);
        drv_ctx.decoder_format = VDEC_CODECTYPE_VC1;
        eCompressionFormat = OMX_VIDEO_CodingWMV;
        codec_type_parse = CODEC_TYPE_VC1;
        output_capability = V4L2_PIX_FMT_VC1_ANNEX_G;
        m_frame_parser.init_start_codes (codec_type_parse);
 } else if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.wmv",\
                OMX_MAX_STRINGNAME_SIZE)) {
        strlcpy((char *)m_cRole, "video_decoder.vc1",OMX_MAX_STRINGNAME_SIZE);
        drv_ctx.decoder_format = VDEC_CODECTYPE_VC1_RCV;
        eCompressionFormat = OMX_VIDEO_CodingWMV;
        codec_type_parse = CODEC_TYPE_VC1;
        output_capability = V4L2_PIX_FMT_VC1_ANNEX_L;
        m_frame_parser.init_start_codes (codec_type_parse);
 } else if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.vp8",    \
                OMX_MAX_STRINGNAME_SIZE)) {
        strlcpy((char *)m_cRole, "video_decoder.vp8",OMX_MAX_STRINGNAME_SIZE);
        output_capability=V4L2_PIX_FMT_VP8;
        eCompressionFormat = OMX_VIDEO_CodingVP8;
        codec_type_parse = CODEC_TYPE_VP8;
        arbitrary_bytes = false;

 } else {
        DEBUG_PRINT_ERROR("ERROR:Unknown Component");
        eRet = OMX_ErrorInvalidComponentName;
 }
 if (eRet == OMX_ErrorNone) {
        OMX_COLOR_FORMATTYPE dest_color_format;
        drv_ctx.output_format = VDEC_YUV_FORMAT_NV12;
 if (eCompressionFormat == (OMX_VIDEO_CODINGTYPE)QOMX_VIDEO_CodingMVC)
            dest_color_format = (OMX_COLOR_FORMATTYPE)
                QOMX_COLOR_FORMATYUV420PackedSemiPlanar32mMultiView;
 else
            dest_color_format = (OMX_COLOR_FORMATTYPE)
            QOMX_COLOR_FORMATYUV420PackedSemiPlanar32m;
 if (!client_buffers.set_color_format(dest_color_format)) {
            DEBUG_PRINT_ERROR("Setting color format failed");
            eRet = OMX_ErrorInsufficientResources;
 }

        capture_capability= V4L2_PIX_FMT_NV12;

 struct v4l2_capability cap;
        ret = ioctl(drv_ctx.video_driver_fd, VIDIOC_QUERYCAP, &cap);
 if (ret) {
            DEBUG_PRINT_ERROR("Failed to query capabilities");
  
 } else {
            DEBUG_PRINT_INFO("Capabilities: driver_name = %s, card = %s, bus_info = %s,"
 " version = %d, capabilities = %x", cap.driver, cap.card,
                    cap.bus_info, cap.version, cap.capabilities);
 }
        ret=0;
        fdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        fdesc.index=0;
 while (ioctl(drv_ctx.video_driver_fd, VIDIOC_ENUM_FMT, &fdesc) == 0) {
            DEBUG_PRINT_HIGH("fmt: description: %s, fmt: %x, flags = %x", fdesc.description,
                    fdesc.pixelformat, fdesc.flags);
            fdesc.index++;
 }
        fdesc.type=V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
        fdesc.index=0;
 while (ioctl(drv_ctx.video_driver_fd, VIDIOC_ENUM_FMT, &fdesc) == 0) {

            DEBUG_PRINT_HIGH("fmt: description: %s, fmt: %x, flags = %x", fdesc.description,
                    fdesc.pixelformat, fdesc.flags);
            fdesc.index++;
 }
        update_resolution(320, 240, 320, 240);
        fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
        fmt.fmt.pix_mp.height = drv_ctx.video_resolution.frame_height;
        fmt.fmt.pix_mp.width = drv_ctx.video_resolution.frame_width;
        fmt.fmt.pix_mp.pixelformat = output_capability;
        ret = ioctl(drv_ctx.video_driver_fd, VIDIOC_S_FMT, &fmt);
 if (ret) {
  
            DEBUG_PRINT_ERROR("Failed to set format on output port");
 return OMX_ErrorInsufficientResources;
 }
        DEBUG_PRINT_HIGH("Set Format was successful");
 if (codec_ambiguous) {
 if (output_capability == V4L2_PIX_FMT_DIVX) {
 struct v4l2_control divx_ctrl;

 if (drv_ctx.decoder_format == VDEC_CODECTYPE_DIVX_4) {
                    divx_ctrl.value = V4L2_MPEG_VIDC_VIDEO_DIVX_FORMAT_4;
 } else if (drv_ctx.decoder_format == VDEC_CODECTYPE_DIVX_5) {
                    divx_ctrl.value = V4L2_MPEG_VIDC_VIDEO_DIVX_FORMAT_5;
 } else {
                    divx_ctrl.value = V4L2_MPEG_VIDC_VIDEO_DIVX_FORMAT_6;
 }

                divx_ctrl.id = V4L2_CID_MPEG_VIDC_VIDEO_DIVX_FORMAT;
                ret = ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL, &divx_ctrl);
 if (ret) {
                    DEBUG_PRINT_ERROR("Failed to set divx version");
 }
 } else {
                DEBUG_PRINT_ERROR("Codec should not be ambiguous");
 }
 }

        property_get("persist.vidc.dec.conceal_color", property_value, DEFAULT_CONCEAL_COLOR);
        m_conceal_color= atoi(property_value);
        DEBUG_PRINT_HIGH("trying to set 0x%u as conceal color\n", (unsigned int)m_conceal_color);
        control.id = V4L2_CID_MPEG_VIDC_VIDEO_CONCEAL_COLOR;
        control.value = m_conceal_color;
        ret = ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL, &control);
 if (ret) {
            DEBUG_PRINT_ERROR("Failed to set conceal color %d\n", ret);
 }

        memset((void *)&frmsize,0,sizeof(frmsize));
        frmsize.index = 0;
        frmsize.pixel_format = output_capability;
        ret = ioctl(drv_ctx.video_driver_fd,
                VIDIOC_ENUM_FRAMESIZES, &frmsize);
 if (ret || frmsize.type != V4L2_FRMSIZE_TYPE_STEPWISE) {
            DEBUG_PRINT_ERROR("Failed to get framesizes");
 return OMX_ErrorHardware;
 }

 if (frmsize.type == V4L2_FRMSIZE_TYPE_STEPWISE) {
            m_decoder_capability.min_width = frmsize.stepwise.min_width;
            m_decoder_capability.max_width = frmsize.stepwise.max_width;
            m_decoder_capability.min_height = frmsize.stepwise.min_height;
            m_decoder_capability.max_height = frmsize.stepwise.max_height;
 }

        memset(&fmt, 0x0, sizeof(struct v4l2_format));
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        fmt.fmt.pix_mp.height = drv_ctx.video_resolution.frame_height;
        fmt.fmt.pix_mp.width = drv_ctx.video_resolution.frame_width;
        fmt.fmt.pix_mp.pixelformat = capture_capability;
        ret = ioctl(drv_ctx.video_driver_fd, VIDIOC_S_FMT, &fmt);
 if (ret) {
  
            DEBUG_PRINT_ERROR("Failed to set format on capture port");
 }
        memset(&framesize, 0, sizeof(OMX_FRAMESIZETYPE));
        framesize.nWidth = drv_ctx.video_resolution.frame_width;
        framesize.nHeight = drv_ctx.video_resolution.frame_height;

        memset(&rectangle, 0, sizeof(OMX_CONFIG_RECTTYPE));
        rectangle.nWidth = drv_ctx.video_resolution.frame_width;
        rectangle.nHeight = drv_ctx.video_resolution.frame_height;

        DEBUG_PRINT_HIGH("Set Format was successful");
 if (secure_mode) {
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_SECURE;
            control.value = 1;
            DEBUG_PRINT_LOW("Omx_vdec:: calling to open secure device %d", ret);
            ret=ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL,&control);
 if (ret) {
                DEBUG_PRINT_ERROR("Omx_vdec:: Unable to open secure device %d", ret);
 return OMX_ErrorInsufficientResources;
 }
 }
 if (output_capability == V4L2_PIX_FMT_H264_MVC) {
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_MVC_BUFFER_LAYOUT;
            control.value = V4L2_MPEG_VIDC_VIDEO_MVC_TOP_BOTTOM;
            ret=ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL,&control);
 if (ret) {
                DEBUG_PRINT_ERROR("Failed to set MVC buffer layout");
 return OMX_ErrorInsufficientResources;
 }
 }

  
        drv_ctx.ip_buf.buffer_type = VDEC_BUFFER_TYPE_INPUT;
        drv_ctx.op_buf.buffer_type = VDEC_BUFFER_TYPE_OUTPUT;
 if (secure_mode) {
            drv_ctx.op_buf.alignment=SZ_1M;
            drv_ctx.ip_buf.alignment=SZ_1M;
 } else {
            drv_ctx.op_buf.alignment=SZ_4K;
            drv_ctx.ip_buf.alignment=SZ_4K;
 }
        drv_ctx.interlace = VDEC_InterlaceFrameProgressive;
        drv_ctx.extradata = 0;
        drv_ctx.picture_order = VDEC_ORDER_DISPLAY;
        control.id = V4L2_CID_MPEG_VIDC_VIDEO_OUTPUT_ORDER;
        control.value = V4L2_MPEG_VIDC_VIDEO_OUTPUT_ORDER_DISPLAY;
        ret = ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL, &control);
        drv_ctx.idr_only_decoding = 0;

        property_get("vidc.debug.turbo", property_value, "0");
 if (atoi(property_value)) {
            DEBUG_PRINT_HIGH("Turbo mode debug property enabled");
            control.id = V4L2_CID_MPEG_VIDC_SET_PERF_LEVEL;
            control.value = V4L2_CID_MPEG_VIDC_PERF_LEVEL_TURBO;
 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL, &control)) {
                DEBUG_PRINT_ERROR("Failed to set turbo mode");
 }
 }

        m_state = OMX_StateLoaded;
#ifdef DEFAULT_EXTRADATA
 if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.avc", OMX_MAX_STRINGNAME_SIZE) &&
 (eRet == OMX_ErrorNone)) {
                DEBUG_PRINT_INFO("Enabling default extradata for %s", drv_ctx.kind);
                enable_extradata(DEFAULT_EXTRADATA, true, true);
 }
#endif
        eRet=get_buffer_req(&drv_ctx.ip_buf);
        DEBUG_PRINT_HIGH("Input Buffer Size =%u",(unsigned int)drv_ctx.ip_buf.buffer_size);
        get_buffer_req(&drv_ctx.op_buf);
 if (drv_ctx.decoder_format == VDEC_CODECTYPE_H264 ||
                drv_ctx.decoder_format == VDEC_CODECTYPE_HEVC ||
                drv_ctx.decoder_format == VDEC_CODECTYPE_MVC) {
                    h264_scratch.nAllocLen = drv_ctx.ip_buf.buffer_size;
                    h264_scratch.pBuffer = (OMX_U8 *)malloc (drv_ctx.ip_buf.buffer_size);
                    h264_scratch.nFilledLen = 0;
                    h264_scratch.nOffset = 0;

 if (h264_scratch.pBuffer == NULL) {
                        DEBUG_PRINT_ERROR("h264_scratch.pBuffer Allocation failed ");
 return OMX_ErrorInsufficientResources;
 }
 }
 if (drv_ctx.decoder_format == VDEC_CODECTYPE_H264 ||
            drv_ctx.decoder_format == VDEC_CODECTYPE_MVC) {
 if (m_frame_parser.mutils == NULL) {
                m_frame_parser.mutils = new H264_Utils();
 if (m_frame_parser.mutils == NULL) {
                    DEBUG_PRINT_ERROR("parser utils Allocation failed ");
                    eRet = OMX_ErrorInsufficientResources;
 } else {
                    m_frame_parser.mutils->initialize_frame_checking_environment();
                    m_frame_parser.mutils->allocate_rbsp_buffer (drv_ctx.ip_buf.buffer_size);
 }
 }

            h264_parser = new h264_stream_parser();
 if (!h264_parser) {
                DEBUG_PRINT_ERROR("ERROR: H264 parser allocation failed!");
                eRet = OMX_ErrorInsufficientResources;
 }
 }

 if (pipe(fds)) {
            DEBUG_PRINT_ERROR("pipe creation failed");
            eRet = OMX_ErrorInsufficientResources;
 } else {
 int temp1[2];
 if (fds[0] == 0 || fds[1] == 0) {
 if (pipe (temp1)) {
                    DEBUG_PRINT_ERROR("pipe creation failed");
 return OMX_ErrorInsufficientResources;
 }
                fds[0] = temp1 [0];
                fds[1] = temp1 [1];
 }
            m_pipe_in = fds[0];
            m_pipe_out = fds[1];
            msg_thread_created = true;
            r = pthread_create(&msg_thread_id,0,message_thread,this);

 if (r < 0) {
                DEBUG_PRINT_ERROR("component_init(): message_thread creation failed");
                msg_thread_created = false;
                eRet = OMX_ErrorInsufficientResources;
 }
 }
 }

 if (eRet != OMX_ErrorNone) {
        DEBUG_PRINT_ERROR("Component Init Failed");
 } else {
        DEBUG_PRINT_INFO("omx_vdec::component_init() success : fd=%d",
                drv_ctx.video_driver_fd);
 }
    control.id = V4L2_CID_MPEG_VIDC_VIDEO_PRIORITY;
    control.value = V4L2_MPEG_VIDC_VIDEO_PRIORITY_REALTIME_DISABLE;

 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL, &control)) {
        DEBUG_PRINT_ERROR("Failed to set Default Priority");
        eRet = OMX_ErrorUnsupportedSetting;
 }
 return eRet;
}
