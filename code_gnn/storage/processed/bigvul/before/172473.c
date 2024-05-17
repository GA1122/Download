bool venc_dev::venc_set_param(void *paramData,OMX_INDEXTYPE index )
{
    DEBUG_PRINT_LOW("venc_set_param:: venc-720p");
 struct v4l2_format fmt;
 struct v4l2_requestbuffers bufreq;
 int ret;

 switch ((int)index) {
 case OMX_IndexParamPortDefinition:
 {
                OMX_PARAM_PORTDEFINITIONTYPE *portDefn;
                portDefn = (OMX_PARAM_PORTDEFINITIONTYPE *) paramData;
                DEBUG_PRINT_LOW("venc_set_param: OMX_IndexParamPortDefinition");

 if (portDefn->nPortIndex == PORT_INDEX_IN) {
 if (!venc_set_encode_framerate(portDefn->format.video.xFramerate, 0)) {
 return false;
 }

 if (!venc_set_color_format(portDefn->format.video.eColorFormat)) {
 return false;
 }
 if (enable_mv_narrow_searchrange &&
 (m_sVenc_cfg.input_width * m_sVenc_cfg.input_height) >=
 (OMX_CORE_1080P_WIDTH * OMX_CORE_1080P_HEIGHT)) {
 if (venc_set_searchrange() == false) {
                            DEBUG_PRINT_ERROR("ERROR: Failed to set search range");
 }
 }
 if (m_sVenc_cfg.input_height != portDefn->format.video.nFrameHeight ||
                            m_sVenc_cfg.input_width != portDefn->format.video.nFrameWidth) {
                        DEBUG_PRINT_LOW("Basic parameter has changed");
                        m_sVenc_cfg.input_height = portDefn->format.video.nFrameHeight;
                        m_sVenc_cfg.input_width = portDefn->format.video.nFrameWidth;
                        fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
                        fmt.fmt.pix_mp.height = m_sVenc_cfg.input_height;
                        fmt.fmt.pix_mp.width = m_sVenc_cfg.input_width;
                        fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_NV12;
                        fmt.fmt.pix_mp.colorspace = V4L2_COLORSPACE_BT878;

 if (ioctl(m_nDriver_fd, VIDIOC_S_FMT, &fmt)) {
                            DEBUG_PRINT_ERROR("VIDIOC_S_FMT OUTPUT_MPLANE Failed");
                            hw_overload = errno == EBUSY;
 return false;
 }

                        m_sInput_buff_property.datasize=fmt.fmt.pix_mp.plane_fmt[0].sizeimage;
                        bufreq.memory = V4L2_MEMORY_USERPTR;
                        bufreq.count = portDefn->nBufferCountActual;
                        bufreq.type=V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;

 if (ioctl(m_nDriver_fd,VIDIOC_REQBUFS, &bufreq)) {
                            DEBUG_PRINT_ERROR("VIDIOC_REQBUFS OUTPUT_MPLANE Failed");
 return false;
 }

 if (bufreq.count == portDefn->nBufferCountActual)
                            m_sInput_buff_property.mincount = m_sInput_buff_property.actualcount = bufreq.count;

 if (portDefn->nBufferCountActual >= m_sInput_buff_property.mincount)
                            m_sInput_buff_property.actualcount = portDefn->nBufferCountActual;
 }

                    DEBUG_PRINT_LOW("input: actual: %u, min: %u, count_req: %u",
 (unsigned int)portDefn->nBufferCountActual, (unsigned int)m_sInput_buff_property.mincount, bufreq.count);
 if (m_sVenc_cfg.input_width * m_sVenc_cfg.input_height >= 3840 * 2160) {
 if (venc_set_perf_mode(V4L2_MPEG_VIDC_VIDEO_PERF_POWER_SAVE) == false) {
                            DEBUG_PRINT_ERROR("ERROR: Failed to set Power save mode");
 }
 }
 } else if (portDefn->nPortIndex == PORT_INDEX_OUT) {
                    m_sVenc_cfg.dvs_height = portDefn->format.video.nFrameHeight;
                    m_sVenc_cfg.dvs_width = portDefn->format.video.nFrameWidth;
                    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
                    fmt.fmt.pix_mp.height = m_sVenc_cfg.dvs_height;
                    fmt.fmt.pix_mp.width = m_sVenc_cfg.dvs_width;
                    fmt.fmt.pix_mp.pixelformat = m_sVenc_cfg.codectype;

 if (ioctl(m_nDriver_fd, VIDIOC_S_FMT, &fmt)) {
                        DEBUG_PRINT_ERROR("VIDIOC_S_FMT CAPTURE_MPLANE Failed");
                        hw_overload = errno == EBUSY;
 return false;
 }

                    m_sOutput_buff_property.datasize = fmt.fmt.pix_mp.plane_fmt[0].sizeimage;

 if (!venc_set_target_bitrate(portDefn->format.video.nBitrate, 0)) {
 return false;
 }

                        m_sOutput_buff_property.actualcount = portDefn->nBufferCountActual;
                        bufreq.memory = V4L2_MEMORY_USERPTR;
                        bufreq.count = portDefn->nBufferCountActual;
                        bufreq.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;

 if (ioctl(m_nDriver_fd,VIDIOC_REQBUFS, &bufreq)) {
                            DEBUG_PRINT_ERROR("ERROR: Request for setting o/p buffer count failed: requested: %u, current: %u",
 (unsigned int)portDefn->nBufferCountActual, (unsigned int)m_sOutput_buff_property.actualcount);
 return false;
 }

 if (bufreq.count == portDefn->nBufferCountActual)
                            m_sOutput_buff_property.mincount = m_sOutput_buff_property.actualcount = bufreq.count;

 if (portDefn->nBufferCountActual >= m_sOutput_buff_property.mincount)
                            m_sOutput_buff_property.actualcount = portDefn->nBufferCountActual;

 if (num_planes > 1)
                            extradata_info.count = m_sOutput_buff_property.actualcount;

                    DEBUG_PRINT_LOW("Output: actual: %u, min: %u, count_req: %u",
 (unsigned int)portDefn->nBufferCountActual, (unsigned int)m_sOutput_buff_property.mincount, bufreq.count);
 } else {
                    DEBUG_PRINT_ERROR("ERROR: Invalid Port Index for OMX_IndexParamPortDefinition");
 }

 break;
 }
 case OMX_IndexParamVideoPortFormat:
 {
                OMX_VIDEO_PARAM_PORTFORMATTYPE *portFmt;
                portFmt =(OMX_VIDEO_PARAM_PORTFORMATTYPE *)paramData;
                DEBUG_PRINT_LOW("venc_set_param: OMX_IndexParamVideoPortFormat");

 if (portFmt->nPortIndex == (OMX_U32) PORT_INDEX_IN) {
 if (!venc_set_color_format(portFmt->eColorFormat)) {
 return false;
 }
 } else if (portFmt->nPortIndex == (OMX_U32) PORT_INDEX_OUT) {
 if (!venc_set_encode_framerate(portFmt->xFramerate, 0)) {
 return false;
 }
 } else {
                    DEBUG_PRINT_ERROR("ERROR: Invalid Port Index for OMX_IndexParamVideoPortFormat");
 }

 break;
 }
 case OMX_IndexParamVideoBitrate:
 {
                OMX_VIDEO_PARAM_BITRATETYPE* pParam;
                pParam = (OMX_VIDEO_PARAM_BITRATETYPE*)paramData;
                DEBUG_PRINT_LOW("venc_set_param: OMX_IndexParamVideoBitrate");

 if (pParam->nPortIndex == (OMX_U32) PORT_INDEX_OUT) {
 if (!venc_set_target_bitrate(pParam->nTargetBitrate, 0)) {
                        DEBUG_PRINT_ERROR("ERROR: Target Bit Rate setting failed");
 return false;
 }

 if (!venc_set_ratectrl_cfg(pParam->eControlRate)) {
                        DEBUG_PRINT_ERROR("ERROR: Rate Control setting failed");
 return false;
 }
 } else {
                    DEBUG_PRINT_ERROR("ERROR: Invalid Port Index for OMX_IndexParamVideoBitrate");
 }

 break;
 }
 case OMX_IndexParamVideoMpeg4:
 {
                OMX_VIDEO_PARAM_MPEG4TYPE* pParam;
                OMX_U32 bFrames = 0;

                pParam = (OMX_VIDEO_PARAM_MPEG4TYPE*)paramData;
                DEBUG_PRINT_LOW("venc_set_param: OMX_IndexParamVideoMpeg4");

 if (pParam->nPortIndex == (OMX_U32) PORT_INDEX_OUT) {
 if (!venc_set_voptiming_cfg(pParam->nTimeIncRes)) {
                        DEBUG_PRINT_ERROR("ERROR: Request for setting vop_timing failed");
 return false;
 }

                    m_profile_set = false;
                    m_level_set = false;

 if (!venc_set_profile_level (pParam->eProfile, pParam->eLevel)) {
                        DEBUG_PRINT_ERROR("ERROR: Unsuccessful in updating Profile and level");
 return false;
 } else {
 if (pParam->eProfile == OMX_VIDEO_MPEG4ProfileAdvancedSimple) {
 if (pParam->nBFrames) {
                                bFrames = pParam->nBFrames;
 }
 } else {
 if (pParam->nBFrames) {
                                DEBUG_PRINT_ERROR("Warning: B frames not supported");
                                bFrames = 0;
 }
 }
 }

 if (!venc_set_intra_period (pParam->nPFrames,bFrames)) {
                        DEBUG_PRINT_ERROR("ERROR: Request for setting intra period failed");
 return false;
 }

 if (!venc_set_multislice_cfg(OMX_IndexParamVideoMpeg4,pParam->nSliceHeaderSpacing)) {
                        DEBUG_PRINT_ERROR("ERROR: Unsuccessful in updating slice_config");
 return false;
 }
 } else {
                    DEBUG_PRINT_ERROR("ERROR: Invalid Port Index for OMX_IndexParamVideoMpeg4");
 }

 break;
 }
 case OMX_IndexParamVideoH263:
 {
                OMX_VIDEO_PARAM_H263TYPE* pParam = (OMX_VIDEO_PARAM_H263TYPE*)paramData;
                DEBUG_PRINT_LOW("venc_set_param: OMX_IndexParamVideoH263");
                OMX_U32 bFrames = 0;

 if (pParam->nPortIndex == (OMX_U32) PORT_INDEX_OUT) {
                    m_profile_set = false;
                    m_level_set = false;

 if (!venc_set_profile_level (pParam->eProfile, pParam->eLevel)) {
                        DEBUG_PRINT_ERROR("ERROR: Unsuccessful in updating Profile and level");
 return false;
 }

 if (pParam->nBFrames)
                        DEBUG_PRINT_ERROR("WARNING: B frame not supported for H.263");

 if (venc_set_intra_period (pParam->nPFrames, bFrames) == false) {
                        DEBUG_PRINT_ERROR("ERROR: Request for setting intra period failed");
 return false;
 }
 } else {
                    DEBUG_PRINT_ERROR("ERROR: Invalid Port Index for OMX_IndexParamVideoH263");
 }

 break;
 }
 case OMX_IndexParamVideoAvc:
 {
                DEBUG_PRINT_LOW("venc_set_param:OMX_IndexParamVideoAvc");
                OMX_VIDEO_PARAM_AVCTYPE* pParam = (OMX_VIDEO_PARAM_AVCTYPE*)paramData;
                OMX_U32 bFrames = 0;

 if (pParam->nPortIndex == (OMX_U32) PORT_INDEX_OUT) {
                    DEBUG_PRINT_LOW("pParam->eProfile :%d ,pParam->eLevel %d",
                            pParam->eProfile,pParam->eLevel);

                    m_profile_set = false;
                    m_level_set = false;

 if (!venc_set_profile_level (pParam->eProfile,pParam->eLevel)) {
                        DEBUG_PRINT_ERROR("ERROR: Unsuccessful in updating Profile and level %d, %d",
                                pParam->eProfile, pParam->eLevel);
 return false;
 } else {
 if ((pParam->eProfile != OMX_VIDEO_AVCProfileBaseline) &&
 (pParam->eProfile != (OMX_VIDEO_AVCPROFILETYPE) QOMX_VIDEO_AVCProfileConstrainedBaseline)) {
 if (pParam->nBFrames) {
                                bFrames = pParam->nBFrames;
 }
 } else {
 if (pParam->nBFrames) {
                                DEBUG_PRINT_ERROR("Warning: B frames not supported");
                                bFrames = 0;
 }
 }
 }

 if (!venc_set_intra_period (pParam->nPFrames, bFrames)) {
                        DEBUG_PRINT_ERROR("ERROR: Request for setting intra period failed");
 return false;
 }

 if (!venc_set_entropy_config (pParam->bEntropyCodingCABAC, pParam->nCabacInitIdc)) {
                        DEBUG_PRINT_ERROR("ERROR: Request for setting Entropy failed");
 return false;
 }

 if (!venc_set_inloop_filter (pParam->eLoopFilterMode)) {
                        DEBUG_PRINT_ERROR("ERROR: Request for setting Inloop filter failed");
 return false;
 }

 if (!venc_set_multislice_cfg(OMX_IndexParamVideoAvc, pParam->nSliceHeaderSpacing)) {
                        DEBUG_PRINT_ERROR("WARNING: Unsuccessful in updating slice_config");
 return false;
 }
 } else {
                    DEBUG_PRINT_ERROR("ERROR: Invalid Port Index for OMX_IndexParamVideoAvc");
 }

 break;
 }
 case (OMX_INDEXTYPE)OMX_IndexParamVideoVp8:
 {
                DEBUG_PRINT_LOW("venc_set_param:OMX_IndexParamVideoVp8");
                OMX_VIDEO_PARAM_VP8TYPE* pParam = (OMX_VIDEO_PARAM_VP8TYPE*)paramData;
 if (!venc_set_profile_level (pParam->eProfile, pParam->eLevel)) {
                    DEBUG_PRINT_ERROR("ERROR: Unsuccessful in updating Profile and level %d, %d",
                                        pParam->eProfile, pParam->eLevel);
 return false;
 }
 if(venc_set_vpx_error_resilience(pParam->bErrorResilientMode) == false) {
                    DEBUG_PRINT_ERROR("ERROR: Failed to set vpx error resilience");
 return false;
 }
 if(!venc_set_ltrmode(1, 1)) {
                   DEBUG_PRINT_ERROR("ERROR: Failed to enable ltrmode");
 return false;
 }

 if (m_codec == OMX_VIDEO_CodingVP8) {
                     DEBUG_PRINT_LOW("Disable Hier-P as LTR is being set");
 if (!venc_set_hier_layers(QOMX_HIERARCHICALCODING_P, 0)) {
                        DEBUG_PRINT_ERROR("Disabling Hier P count failed");
 }
 }

 break;
 }
 case (OMX_INDEXTYPE)OMX_IndexParamVideoHevc:
 {
                DEBUG_PRINT_LOW("venc_set_param:OMX_IndexParamVideoHevc");
                OMX_VIDEO_PARAM_HEVCTYPE* pParam = (OMX_VIDEO_PARAM_HEVCTYPE*)paramData;
 if (!venc_set_profile_level (pParam->eProfile, pParam->eLevel)) {
                    DEBUG_PRINT_ERROR("ERROR: Unsuccessful in updating Profile and level %d, %d",
                                        pParam->eProfile, pParam->eLevel);
 return false;
 }
 if (!venc_set_inloop_filter(OMX_VIDEO_AVCLoopFilterEnable))
                    DEBUG_PRINT_HIGH("WARN: Request for setting Inloop filter failed for HEVC encoder");

 break;
 }
 case OMX_IndexParamVideoIntraRefresh:
 {
                DEBUG_PRINT_LOW("venc_set_param:OMX_IndexParamVideoIntraRefresh");
                OMX_VIDEO_PARAM_INTRAREFRESHTYPE *intra_refresh =
 (OMX_VIDEO_PARAM_INTRAREFRESHTYPE *)paramData;

 if (intra_refresh->nPortIndex == (OMX_U32) PORT_INDEX_OUT) {
 if (venc_set_intra_refresh(intra_refresh->eRefreshMode, intra_refresh->nCirMBs) == false) {
                        DEBUG_PRINT_ERROR("ERROR: Setting Intra refresh failed");
 return false;
 }
 } else {
                    DEBUG_PRINT_ERROR("ERROR: Invalid Port Index for OMX_IndexParamVideoIntraRefresh");
 }

 break;
 }
 case OMX_IndexParamVideoErrorCorrection:
 {
                DEBUG_PRINT_LOW("venc_set_param:OMX_IndexParamVideoErrorCorrection");
                OMX_VIDEO_PARAM_ERRORCORRECTIONTYPE *error_resilience =
 (OMX_VIDEO_PARAM_ERRORCORRECTIONTYPE *)paramData;

 if (error_resilience->nPortIndex == (OMX_U32) PORT_INDEX_OUT) {
 if (venc_set_error_resilience(error_resilience) == false) {
                        DEBUG_PRINT_ERROR("ERROR: Setting Intra refresh failed");
 return false;
 }
 } else {
                    DEBUG_PRINT_ERROR("ERROR: Invalid Port Index for OMX_IndexParamVideoErrorCorrection");
 }

 break;
 }
 case OMX_IndexParamVideoProfileLevelCurrent:
 {
                DEBUG_PRINT_LOW("venc_set_param:OMX_IndexParamVideoProfileLevelCurrent");
                OMX_VIDEO_PARAM_PROFILELEVELTYPE *profile_level =
 (OMX_VIDEO_PARAM_PROFILELEVELTYPE *)paramData;

 if (profile_level->nPortIndex == (OMX_U32) PORT_INDEX_OUT) {
                    m_profile_set = false;
                    m_level_set = false;

 if (!venc_set_profile_level (profile_level->eProfile,
                                profile_level->eLevel)) {
                        DEBUG_PRINT_ERROR("WARNING: Unsuccessful in updating Profile and level");
 return false;
 }
 } else {
                    DEBUG_PRINT_ERROR("ERROR: Invalid Port Index for OMX_IndexParamVideoProfileLevelCurrent");
 }

 break;
 }
 case OMX_IndexParamVideoQuantization:
 {
                DEBUG_PRINT_LOW("venc_set_param:OMX_IndexParamVideoQuantization");
                OMX_VIDEO_PARAM_QUANTIZATIONTYPE *session_qp =
 (OMX_VIDEO_PARAM_QUANTIZATIONTYPE *)paramData;
 if (session_qp->nPortIndex == (OMX_U32) PORT_INDEX_OUT) {
 if (venc_set_session_qp (session_qp->nQpI,
                                session_qp->nQpP,
                                session_qp->nQpB) == false) {
                        DEBUG_PRINT_ERROR("ERROR: Setting Session QP failed");
 return false;
 }
 } else {
                    DEBUG_PRINT_ERROR("ERROR: Invalid Port Index for OMX_IndexParamVideoQuantization");
 }

 break;
 }
 case QOMX_IndexParamVideoInitialQp:
 {
                QOMX_EXTNINDEX_VIDEO_INITIALQP * initqp =
 (QOMX_EXTNINDEX_VIDEO_INITIALQP *)paramData;
 if (initqp->bEnableInitQp) {
                    DEBUG_PRINT_LOW("Enable initial QP: %d", (int)initqp->bEnableInitQp);
 if(venc_enable_initial_qp(initqp) == false) {
                       DEBUG_PRINT_ERROR("ERROR: Failed to enable initial QP");
 return OMX_ErrorUnsupportedSetting;
 }
 } else
                    DEBUG_PRINT_ERROR("ERROR: setting QOMX_IndexParamVideoEnableInitialQp");
 break;
 }
 case OMX_QcomIndexParamVideoQPRange:
 {
                DEBUG_PRINT_LOW("venc_set_param:OMX_QcomIndexParamVideoQPRange");
                OMX_QCOM_VIDEO_PARAM_QPRANGETYPE *session_qp_range =
 (OMX_QCOM_VIDEO_PARAM_QPRANGETYPE *)paramData;

 if(session_qp_range->nPortIndex == (OMX_U32)PORT_INDEX_OUT) {
 if(venc_set_session_qp_range (session_qp_range->minQP,
                                session_qp_range->maxQP) == false) {
                        DEBUG_PRINT_ERROR("ERROR: Setting QP Range[%u %u] failed",
 (unsigned int)session_qp_range->minQP, (unsigned int)session_qp_range->maxQP);
 return false;
 } else {
                        session_qp_values.minqp = session_qp_range->minQP;
                        session_qp_values.maxqp = session_qp_range->maxQP;
 }
 } else {
                    DEBUG_PRINT_ERROR("ERROR: Invalid Port Index for OMX_QcomIndexParamVideoQPRange");
 }

 break;
 }
 case OMX_QcomIndexEnableSliceDeliveryMode:
 {
                QOMX_EXTNINDEX_PARAMTYPE* pParam =
 (QOMX_EXTNINDEX_PARAMTYPE*)paramData;

 if (pParam->nPortIndex == PORT_INDEX_OUT) {
 if (venc_set_slice_delivery_mode(pParam->bEnable) == false) {
                        DEBUG_PRINT_ERROR("Setting slice delivery mode failed");
 return OMX_ErrorUnsupportedSetting;
 }
 } else {
                    DEBUG_PRINT_ERROR("OMX_QcomIndexEnableSliceDeliveryMode "
 "called on wrong port(%u)", (unsigned int)pParam->nPortIndex);
 return OMX_ErrorBadPortIndex;
 }

 break;
 }
 case OMX_ExtraDataVideoEncoderSliceInfo:
 {
                DEBUG_PRINT_LOW("venc_set_param: OMX_ExtraDataVideoEncoderSliceInfo");
                OMX_BOOL extra_data = *(OMX_BOOL *)(paramData);

 if (venc_set_extradata(OMX_ExtraDataVideoEncoderSliceInfo, extra_data) == false) {
                    DEBUG_PRINT_ERROR("ERROR: Setting OMX_ExtraDataVideoEncoderSliceInfo failed");
 return false;
 }

                extradata = true;
 break;
 }
 case OMX_ExtraDataVideoEncoderMBInfo:
 {
                DEBUG_PRINT_LOW("venc_set_param: OMX_ExtraDataVideoEncoderMBInfo");
                OMX_BOOL extra_data = *(OMX_BOOL *)(paramData);

 if (venc_set_extradata(OMX_ExtraDataVideoEncoderMBInfo, extra_data) == false) {
                    DEBUG_PRINT_ERROR("ERROR: Setting OMX_ExtraDataVideoEncoderMBInfo failed");
 return false;
 }

                extradata = true;
 break;
 }
 case OMX_QcomIndexParamSequenceHeaderWithIDR:
 {
 PrependSPSPPSToIDRFramesParams * pParam =
 (PrependSPSPPSToIDRFramesParams *)paramData;

                DEBUG_PRINT_LOW("set inband sps/pps: %d", pParam->bEnable);
 if(venc_set_inband_video_header(pParam->bEnable) == false) {
                    DEBUG_PRINT_ERROR("ERROR: set inband sps/pps failed");
 return OMX_ErrorUnsupportedSetting;
 }

 break;
 }
 case OMX_QcomIndexParamH264AUDelimiter:
 {
                OMX_QCOM_VIDEO_CONFIG_H264_AUD * pParam =
 (OMX_QCOM_VIDEO_CONFIG_H264_AUD *)paramData;

                DEBUG_PRINT_LOW("set AU delimiters: %d", pParam->bEnable);
 if(venc_set_au_delimiter(pParam->bEnable) == false) {
                    DEBUG_PRINT_ERROR("ERROR: set H264 AU delimiter failed");
 return OMX_ErrorUnsupportedSetting;
 }

 break;
 }
 case OMX_QcomIndexHierarchicalStructure:
 {
               QOMX_VIDEO_HIERARCHICALLAYERS* pParam =
 (QOMX_VIDEO_HIERARCHICALLAYERS*)paramData;

 if (pParam->nPortIndex == PORT_INDEX_OUT) {
 if (!venc_set_hier_layers(pParam->eHierarchicalCodingType, pParam->nNumLayers)) {
                        DEBUG_PRINT_ERROR("Setting Hier P count failed");
 return false;
 }
 } else {
                    DEBUG_PRINT_ERROR("OMX_QcomIndexHierarchicalStructure called on wrong port(%d)", (int)pParam->nPortIndex);
 return false;
 }

 if (m_codec == OMX_VIDEO_CodingVP8) {
                    DEBUG_PRINT_LOW("Disable LTR as HIER-P is being set");
 if(!venc_set_ltrmode(0, 1)) {
                         DEBUG_PRINT_ERROR("ERROR: Failed to disable ltrmode");
 }
 }
 break;
 }
 case OMX_QcomIndexParamPerfLevel:
 {
                OMX_QCOM_VIDEO_PARAM_PERF_LEVEL *pParam =
 (OMX_QCOM_VIDEO_PARAM_PERF_LEVEL *)paramData;
                DEBUG_PRINT_LOW("Set perf level: %d", pParam->ePerfLevel);
 if(!venc_set_perf_level(pParam->ePerfLevel)) {
                    DEBUG_PRINT_ERROR("ERROR: Failed to set perf level to %d", pParam->ePerfLevel);
 return false;
 } else {
                    performance_level.perflevel = (unsigned int) pParam->ePerfLevel;
 }
 break;
 }
 case OMX_QcomIndexParamH264VUITimingInfo:
 {
                OMX_QCOM_VIDEO_PARAM_VUI_TIMING_INFO *pParam =
 (OMX_QCOM_VIDEO_PARAM_VUI_TIMING_INFO *)paramData;
                DEBUG_PRINT_LOW("Set VUI timing info: %d", pParam->bEnable);
 if(venc_set_vui_timing_info(pParam->bEnable) == false) {
                    DEBUG_PRINT_ERROR("ERROR: Failed to set vui timing info to %d", pParam->bEnable);
 return false;
 } else {
                    vui_timing_info.enabled = (unsigned int) pParam->bEnable;
 }
 break;
 }
 case OMX_QcomIndexParamPeakBitrate:
 {
                OMX_QCOM_VIDEO_PARAM_PEAK_BITRATE *pParam =
 (OMX_QCOM_VIDEO_PARAM_PEAK_BITRATE *)paramData;
                DEBUG_PRINT_LOW("Set peak bitrate: %u", (unsigned int)pParam->nPeakBitrate);
 if(venc_set_peak_bitrate(pParam->nPeakBitrate) == false) {
                    DEBUG_PRINT_ERROR("ERROR: Failed to set peak bitrate to %u", (unsigned int)pParam->nPeakBitrate);
 return false;
 } else {
                    peak_bitrate.peakbitrate = (unsigned int) pParam->nPeakBitrate;
 }
 break;
 }
 case OMX_QcomIndexParamSetMVSearchrange:
 {
               DEBUG_PRINT_LOW("venc_set_config: OMX_QcomIndexParamSetMVSearchrange");
               is_searchrange_set = true;
 if (!venc_set_searchrange()) {
                   DEBUG_PRINT_ERROR("ERROR: Failed to set search range");
 return false;
 }
 }
 break;
 case OMX_QcomIndexParamVideoLTRCount:
 {
                DEBUG_PRINT_LOW("venc_set_param: OMX_QcomIndexParamVideoLTRCount");
                OMX_QCOM_VIDEO_PARAM_LTRCOUNT_TYPE* pParam =
 (OMX_QCOM_VIDEO_PARAM_LTRCOUNT_TYPE*)paramData;
 if (pParam->nCount > 0) {
 if (venc_set_ltrmode(1, pParam->nCount) == false) {
                        DEBUG_PRINT_ERROR("ERROR: Enable LTR mode failed");
 return false;
 }
 } else {
 if (venc_set_ltrmode(0, 0) == false) {
                        DEBUG_PRINT_ERROR("ERROR: Disable LTR mode failed");
 return false;
 }
 }
 break;
 }
 case OMX_QcomIndexParamVideoHybridHierpMode:
 {
                QOMX_EXTNINDEX_VIDEO_HYBRID_HP_MODE* pParam =
 (QOMX_EXTNINDEX_VIDEO_HYBRID_HP_MODE*)paramData;

 if (!venc_set_hybrid_hierp(pParam->nHpLayers)) {
                     DEBUG_PRINT_ERROR("Setting hybrid Hier-P mode failed");
 return OMX_ErrorUnsupportedSetting;
 }
 break;
 }
 case OMX_IndexParamVideoSliceFMO:
 default:
            DEBUG_PRINT_ERROR("ERROR: Unsupported parameter in venc_set_param: %u",
                    index);
 break;
 }

 return true;
}
