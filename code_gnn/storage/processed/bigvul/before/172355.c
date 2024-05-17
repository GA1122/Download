OMX_ERRORTYPE omx_video::get_supported_profile_level(OMX_VIDEO_PARAM_PROFILELEVELTYPE *profileLevelType)
{
    OMX_ERRORTYPE eRet = OMX_ErrorNone;
 if (!profileLevelType)
 return OMX_ErrorBadParameter;

 if (profileLevelType->nPortIndex == 1) {
 if (m_sOutPortDef.format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
 if (profileLevelType->nProfileIndex == 0) {
                profileLevelType->eProfile = OMX_VIDEO_AVCProfileBaseline;
                profileLevelType->eLevel   = OMX_VIDEO_AVCLevel31;
 } else if (profileLevelType->nProfileIndex == 1) {
                profileLevelType->eProfile = OMX_VIDEO_AVCProfileMain;
                profileLevelType->eLevel   = OMX_VIDEO_AVCLevel31;
 } else if (profileLevelType->nProfileIndex == 2) {
                profileLevelType->eProfile = OMX_VIDEO_AVCProfileHigh;
                profileLevelType->eLevel   = OMX_VIDEO_AVCLevel31;
 } else {
                DEBUG_PRINT_LOW("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported nProfileIndex ret NoMore %d",
 (int)profileLevelType->nProfileIndex);
                eRet = OMX_ErrorNoMore;
 }
 } else if (m_sOutPortDef.format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
 if (profileLevelType->nProfileIndex == 0) {
                profileLevelType->eProfile = OMX_VIDEO_H263ProfileBaseline;
                profileLevelType->eLevel   = OMX_VIDEO_H263Level70;
 } else {
                DEBUG_PRINT_ERROR("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported nProfileIndex ret NoMore %d", (int)profileLevelType->nProfileIndex);
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
                DEBUG_PRINT_ERROR("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported nProfileIndex ret NoMore %d", (int)profileLevelType->nProfileIndex);
                eRet = OMX_ErrorNoMore;
 }
 }
 } else {
        DEBUG_PRINT_ERROR("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported should be queried on Input port only %d", (int)profileLevelType->nPortIndex);
        eRet = OMX_ErrorBadPortIndex;
 }
    DEBUG_PRINT_LOW("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported for Input port returned Profile:%d, Level:%d",
 (int)profileLevelType->eProfile, (int)profileLevelType->eLevel);
 return eRet;
}
