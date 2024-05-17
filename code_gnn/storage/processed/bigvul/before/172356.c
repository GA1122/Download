OMX_ERRORTYPE omx_video::get_supported_profile_level(OMX_VIDEO_PARAM_PROFILELEVELTYPE *profileLevelType)
{
    OMX_ERRORTYPE eRet = OMX_ErrorNone;
 if (!profileLevelType)
 return OMX_ErrorBadParameter;

 if (profileLevelType->nPortIndex == 1) {
 if (m_sOutPortDef.format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
#if defined _MSM8974_ && !defined _MSM8226_
 if (profileLevelType->nProfileIndex == 0) {
                profileLevelType->eProfile = OMX_VIDEO_AVCProfileBaseline;
                profileLevelType->eLevel   = OMX_VIDEO_AVCLevel52;
 } else if (profileLevelType->nProfileIndex == 1) {
                profileLevelType->eProfile = OMX_VIDEO_AVCProfileMain;
                profileLevelType->eLevel   = OMX_VIDEO_AVCLevel52;
 } else if (profileLevelType->nProfileIndex == 2) {
                profileLevelType->eProfile = OMX_VIDEO_AVCProfileHigh;
                profileLevelType->eLevel   = OMX_VIDEO_AVCLevel52;
 } else if (profileLevelType->nProfileIndex == 3) {
                profileLevelType->eProfile = QOMX_VIDEO_AVCProfileConstrainedBaseline;
                profileLevelType->eLevel   = OMX_VIDEO_AVCLevel52;
 } else {
                DEBUG_PRINT_LOW("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported nProfileIndex ret NoMore %u",
 (unsigned int)profileLevelType->nProfileIndex);
                eRet = OMX_ErrorNoMore;
 }
#else
 if (profileLevelType->nProfileIndex == 0) {
                profileLevelType->eProfile = OMX_VIDEO_AVCProfileBaseline;
                profileLevelType->eLevel   = OMX_VIDEO_AVCLevel4;

 } else if (profileLevelType->nProfileIndex == 1) {
                profileLevelType->eProfile = OMX_VIDEO_AVCProfileMain;
                profileLevelType->eLevel   = OMX_VIDEO_AVCLevel4;
 } else if (profileLevelType->nProfileIndex == 2) {
                profileLevelType->eProfile = OMX_VIDEO_AVCProfileHigh;
                profileLevelType->eLevel   = OMX_VIDEO_AVCLevel4;
#ifdef _MSM8226_
 } else if (profileLevelType->nProfileIndex == 3) {
                profileLevelType->eProfile = QOMX_VIDEO_AVCProfileConstrainedBaseline;
                profileLevelType->eLevel   = OMX_VIDEO_AVCLevel4;
#endif
 } else {
                DEBUG_PRINT_LOW("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported nProfileIndex ret NoMore %d",
 (int)profileLevelType->nProfileIndex);
                eRet = OMX_ErrorNoMore;
 }
#endif
 } else if (m_sOutPortDef.format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
 if (profileLevelType->nProfileIndex == 0) {
                profileLevelType->eProfile = OMX_VIDEO_H263ProfileBaseline;
                profileLevelType->eLevel   = OMX_VIDEO_H263Level70;
 } else {
                DEBUG_PRINT_ERROR("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported nProfileIndex ret NoMore %u", (unsigned int)profileLevelType->nProfileIndex);
                eRet = OMX_ErrorNoMore;
 }
 } else if (m_sOutPortDef.format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4) {
 if (profileLevelType->nProfileIndex == 0) {
                profileLevelType->eProfile = OMX_VIDEO_MPEG4ProfileSimple;
                profileLevelType->eLevel   = OMX_VIDEO_MPEG4Level5;
 } else if (profileLevelType->nProfileIndex == 1) {
                profileLevelType->eProfile = OMX_VIDEO_MPEG4ProfileAdvancedSimple;
                profileLevelType->eLevel   = OMX_VIDEO_MPEG4Level5;
 } else {
                DEBUG_PRINT_ERROR("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported nProfileIndex ret NoMore %u", (unsigned int)profileLevelType->nProfileIndex);
                eRet = OMX_ErrorNoMore;
 }
 } else if (m_sOutPortDef.format.video.eCompressionFormat == OMX_VIDEO_CodingVP8) {
 if (profileLevelType->nProfileIndex == 0) {
                profileLevelType->eProfile = OMX_VIDEO_VP8ProfileMain;
                profileLevelType->eLevel   = OMX_VIDEO_VP8Level_Version0;
 } else if (profileLevelType->nProfileIndex == 1) {
                profileLevelType->eProfile = OMX_VIDEO_VP8ProfileMain;
                profileLevelType->eLevel   = OMX_VIDEO_VP8Level_Version1;
 } else {
                DEBUG_PRINT_LOW("VP8: get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported nProfileIndex ret NoMore %u",
 (unsigned int)profileLevelType->nProfileIndex);
                eRet = OMX_ErrorNoMore;
 }
 } else if (m_sOutPortDef.format.video.eCompressionFormat == OMX_VIDEO_CodingHEVC) {
 if (profileLevelType->nProfileIndex == 0) {
                profileLevelType->eProfile =  OMX_VIDEO_HEVCProfileMain;
                profileLevelType->eLevel   =  OMX_VIDEO_HEVCMainTierLevel52;
 } else if (profileLevelType->nProfileIndex == 1) {
                profileLevelType->eProfile =  OMX_VIDEO_HEVCProfileMain10;
                profileLevelType->eLevel   =  OMX_VIDEO_HEVCMainTierLevel52;
 } else {
                DEBUG_PRINT_LOW("HEVC: get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported nProfileIndex ret NoMore %u",
 (unsigned int)profileLevelType->nProfileIndex);
                eRet = OMX_ErrorNoMore;
 }
 } else {
            DEBUG_PRINT_ERROR("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported ret NoMore");
            eRet = OMX_ErrorNoMore;
 }
 } else {
        DEBUG_PRINT_ERROR("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported should be queried on Input port only %u", (unsigned int)profileLevelType->nPortIndex);
        eRet = OMX_ErrorBadPortIndex;
 }
    DEBUG_PRINT_LOW("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported for Input port returned Profile:%u, Level:%u",
 (unsigned int)profileLevelType->eProfile, (unsigned int)profileLevelType->eLevel);
 return eRet;
}
