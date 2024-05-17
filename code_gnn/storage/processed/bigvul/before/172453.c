bool venc_dev::venc_set_config(void *configData, OMX_INDEXTYPE index)
{

    DEBUG_PRINT_LOW("Inside venc_set_config");

 switch ((int)index) {
 case OMX_IndexConfigVideoBitrate:
 {
                OMX_VIDEO_CONFIG_BITRATETYPE *bit_rate = (OMX_VIDEO_CONFIG_BITRATETYPE *)
                    configData;
                DEBUG_PRINT_LOW("venc_set_config: OMX_IndexConfigVideoBitrate");

 if (bit_rate->nPortIndex == (OMX_U32)PORT_INDEX_OUT) {
 if (venc_set_target_bitrate(bit_rate->nEncodeBitrate, 1) == false) {
                        DEBUG_PRINT_ERROR("ERROR: Setting Target Bit rate failed");
 return false;
 }
 } else {
                    DEBUG_PRINT_ERROR("ERROR: Invalid Port Index for OMX_IndexConfigVideoBitrate");
 }

 break;
 }
 case OMX_IndexConfigVideoFramerate:
 {
                OMX_CONFIG_FRAMERATETYPE *frame_rate = (OMX_CONFIG_FRAMERATETYPE *)
                    configData;
                DEBUG_PRINT_LOW("venc_set_config: OMX_IndexConfigVideoFramerate");

 if (frame_rate->nPortIndex == (OMX_U32)PORT_INDEX_OUT) {
 if (venc_set_encode_framerate(frame_rate->xEncodeFramerate, 1) == false) {
                        DEBUG_PRINT_ERROR("ERROR: Setting Encode Framerate failed");
 return false;
 }
 } else {
                    DEBUG_PRINT_ERROR("ERROR: Invalid Port Index for OMX_IndexConfigVideoFramerate");
 }

 break;
 }
 case QOMX_IndexConfigVideoIntraperiod:
 {
                DEBUG_PRINT_LOW("venc_set_param:QOMX_IndexConfigVideoIntraperiod");
                QOMX_VIDEO_INTRAPERIODTYPE *intraperiod =
 (QOMX_VIDEO_INTRAPERIODTYPE *)configData;

 if (intraperiod->nPortIndex == (OMX_U32) PORT_INDEX_OUT) {
 if (venc_set_intra_period(intraperiod->nPFrames, intraperiod->nBFrames) == false) {
                        DEBUG_PRINT_ERROR("ERROR: Request for setting intra period failed");
 return false;
 }
 }

 break;
 }
 case OMX_IndexConfigVideoIntraVOPRefresh:
 {
                OMX_CONFIG_INTRAREFRESHVOPTYPE *intra_vop_refresh = (OMX_CONFIG_INTRAREFRESHVOPTYPE *)
                    configData;
                DEBUG_PRINT_LOW("venc_set_config: OMX_IndexConfigVideoIntraVOPRefresh");

 if (intra_vop_refresh->nPortIndex == (OMX_U32)PORT_INDEX_OUT) {
 if (venc_set_intra_vop_refresh(intra_vop_refresh->IntraRefreshVOP) == false) {
                        DEBUG_PRINT_ERROR("ERROR: Setting Encode Framerate failed");
 return false;
 }
 } else {
                    DEBUG_PRINT_ERROR("ERROR: Invalid Port Index for OMX_IndexConfigVideoFramerate");
 }

 break;
 }
 case OMX_IndexConfigCommonRotate:
 {
                OMX_CONFIG_ROTATIONTYPE *config_rotation =
 reinterpret_cast<OMX_CONFIG_ROTATIONTYPE*>(configData);
                OMX_U32 nFrameWidth;
 if (!config_rotation) {
 return false;
 }
 if (true == deinterlace_enabled) {
                    DEBUG_PRINT_ERROR("ERROR: Rotation is not supported with deinterlacing");
 return false;
 }
                DEBUG_PRINT_HIGH("venc_set_config: updating the new Dims");
                nFrameWidth = m_sVenc_cfg.dvs_width;
                m_sVenc_cfg.dvs_width  = m_sVenc_cfg.dvs_height;
                m_sVenc_cfg.dvs_height = nFrameWidth;

 if(venc_set_vpe_rotation(config_rotation->nRotation) == false) {
                    DEBUG_PRINT_ERROR("ERROR: Dimension Change for Rotation failed");
 return false;
 }

 break;
 }
 case OMX_IndexConfigVideoAVCIntraPeriod:
 {
                OMX_VIDEO_CONFIG_AVCINTRAPERIOD *avc_iperiod = (OMX_VIDEO_CONFIG_AVCINTRAPERIOD*) configData;
                DEBUG_PRINT_LOW("venc_set_param: OMX_IndexConfigVideoAVCIntraPeriod");

 if (venc_set_idr_period(avc_iperiod->nPFrames, avc_iperiod->nIDRPeriod)
 == false) {
                    DEBUG_PRINT_ERROR("ERROR: Setting "
 "OMX_IndexConfigVideoAVCIntraPeriod failed");
 return false;
 }
 break;
 }
 case OMX_IndexConfigCommonDeinterlace:
 {
                OMX_VIDEO_CONFIG_DEINTERLACE *deinterlace = (OMX_VIDEO_CONFIG_DEINTERLACE *) configData;
                DEBUG_PRINT_LOW("venc_set_config: OMX_IndexConfigCommonDeinterlace");
 if(deinterlace->nPortIndex == (OMX_U32)PORT_INDEX_OUT) {
 if (m_sVenc_cfg.dvs_width == m_sVenc_cfg.input_height &&
                        m_sVenc_cfg.dvs_height == m_sVenc_cfg.input_width)
 {
                        DEBUG_PRINT_ERROR("ERROR: Deinterlace not supported with rotation");
 return false;
 }
 if(venc_set_deinterlace(deinterlace->nEnable) == false) {
                        DEBUG_PRINT_ERROR("ERROR: Setting Deinterlace failed");
 return false;
 }
 } else {
                DEBUG_PRINT_ERROR("ERROR: Invalid Port Index for OMX_IndexConfigCommonDeinterlace");
 }
 break;
 }
 case OMX_IndexConfigVideoVp8ReferenceFrame:
 {
               OMX_VIDEO_VP8REFERENCEFRAMETYPE* vp8refframe = (OMX_VIDEO_VP8REFERENCEFRAMETYPE*) configData;
                DEBUG_PRINT_LOW("venc_set_config: OMX_IndexConfigVideoVp8ReferenceFrame");
 if ((vp8refframe->nPortIndex == (OMX_U32)PORT_INDEX_IN) &&
 (vp8refframe->bUseGoldenFrame)) {
 if(venc_set_useltr(0x1) == false) {
                        DEBUG_PRINT_ERROR("ERROR: use goldenframe failed");
 return false;
 }
 } else if((vp8refframe->nPortIndex == (OMX_U32)PORT_INDEX_IN) &&
 (vp8refframe->bGoldenFrameRefresh)) {
 if(venc_set_markltr(0x1) == false) {
                        DEBUG_PRINT_ERROR("ERROR: Setting goldenframe failed");
 return false;
 }
 } else {
                    DEBUG_PRINT_ERROR("ERROR: Invalid Port Index for OMX_IndexConfigVideoVp8ReferenceFrame");
 }
 break;
 }
 case OMX_QcomIndexConfigVideoLTRUse:
 {
                OMX_QCOM_VIDEO_CONFIG_LTRUSE_TYPE* pParam = (OMX_QCOM_VIDEO_CONFIG_LTRUSE_TYPE*)configData;
                DEBUG_PRINT_LOW("venc_set_config: OMX_QcomIndexConfigVideoLTRUse");
 if (pParam->nPortIndex == (OMX_U32)PORT_INDEX_IN) {
 if (venc_set_useltr(pParam->nID) == false) {
                        DEBUG_PRINT_ERROR("ERROR: Use LTR failed");
 return false;
 }
 } else {
                    DEBUG_PRINT_ERROR("ERROR: Invalid Port Index for OMX_QcomIndexConfigVideoLTRUse");
 }
 break;
 }
 case OMX_QcomIndexConfigVideoLTRMark:
 {
                OMX_QCOM_VIDEO_CONFIG_LTRMARK_TYPE* pParam = (OMX_QCOM_VIDEO_CONFIG_LTRMARK_TYPE*)configData;
                DEBUG_PRINT_LOW("venc_set_config: OMX_QcomIndexConfigVideoLTRMark");
 if (pParam->nPortIndex == (OMX_U32)PORT_INDEX_IN) {
 if (venc_set_markltr(pParam->nID) == false) {
                        DEBUG_PRINT_ERROR("ERROR: Mark LTR failed");
 return false;
 }
 } else {
                    DEBUG_PRINT_ERROR("ERROR: Invalid Port Index for OMX_QcomIndexConfigVideoLTRMark");
 }
 break;
 }
 case OMX_QcomIndexConfigPerfLevel:
 {
                OMX_QCOM_VIDEO_CONFIG_PERF_LEVEL *perf =
 (OMX_QCOM_VIDEO_CONFIG_PERF_LEVEL *)configData;
                DEBUG_PRINT_LOW("Set perf level: %d", perf->ePerfLevel);
 if (!venc_set_perf_level(perf->ePerfLevel)) {
                    DEBUG_PRINT_ERROR("ERROR: Failed to set perf level to %d", perf->ePerfLevel);
 return false;
 } else {
                    performance_level.perflevel = (unsigned int) perf->ePerfLevel;
 }
 break;
 }
 case OMX_QcomIndexConfigVideoVencPerfMode:
 {
                QOMX_EXTNINDEX_VIDEO_PERFMODE *pParam = (QOMX_EXTNINDEX_VIDEO_PERFMODE *) configData;
                DEBUG_PRINT_LOW("venc_set_config: OMX_QcomIndexConfigVideoVencPerfMode");
 if (venc_set_perf_mode(pParam->nPerfMode) == false) {
                    DEBUG_PRINT_ERROR("Failed to set V4L2_CID_MPEG_VIDC_VIDEO_PERF_MODE");
 return false;
 }
 break;
 }
 case OMX_IndexConfigPriority:
 {
                OMX_PARAM_U32TYPE *priority = (OMX_PARAM_U32TYPE *)configData;
                DEBUG_PRINT_LOW("Set_config: priority %u",priority->nU32);
 if (!venc_set_session_priority(priority->nU32)) {
                    DEBUG_PRINT_ERROR("Failed to set priority");
 return false;
 }
 break;
 }
 case OMX_IndexConfigOperatingRate:
 {
                OMX_PARAM_U32TYPE *rate = (OMX_PARAM_U32TYPE *)configData;
                DEBUG_PRINT_LOW("Set_config: operating rate %d", rate->nU32);
 if (!venc_set_operatingrate(rate->nU32)) {
                    DEBUG_PRINT_ERROR("Failed to set operating rate");
 return false;
 }
 break;
 }
 default:
            DEBUG_PRINT_ERROR("Unsupported config index = %u", index);
 break;
 }

 return true;
}
