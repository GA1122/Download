OMX_ERRORTYPE omx_vdec::get_supported_profile_level_for_1080p(OMX_VIDEO_PARAM_PROFILELEVELTYPE *profileLevelType)
{
    OMX_ERRORTYPE eRet = OMX_ErrorNone;
 if (!profileLevelType)
 return OMX_ErrorBadParameter;

 if (profileLevelType->nPortIndex == 0) {
 if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.avc",OMX_MAX_STRINGNAME_SIZE)) {
 if (profileLevelType->nProfileIndex == 0) {
                profileLevelType->eProfile = OMX_VIDEO_AVCProfileBaseline;
                profileLevelType->eLevel   = OMX_VIDEO_AVCLevel4;

 } else if (profileLevelType->nProfileIndex == 1) {
                profileLevelType->eProfile = OMX_VIDEO_AVCProfileMain;
                profileLevelType->eLevel   = OMX_VIDEO_AVCLevel4;
 } else if (profileLevelType->nProfileIndex == 2) {
                profileLevelType->eProfile = OMX_VIDEO_AVCProfileHigh;
                profileLevelType->eLevel   = OMX_VIDEO_AVCLevel4;
 } else {
                DEBUG_PRINT_LOW("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported nProfileIndex ret NoMore %u",
 (unsigned int)profileLevelType->nProfileIndex);
                eRet = OMX_ErrorNoMore;
 }
 } else if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.mvc", OMX_MAX_STRINGNAME_SIZE)) {
 if (profileLevelType->nProfileIndex == 0) {
                profileLevelType->eProfile = QOMX_VIDEO_MVCProfileStereoHigh;
                profileLevelType->eLevel   = QOMX_VIDEO_MVCLevel51;
 } else {
                DEBUG_PRINT_LOW("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported nProfileIndex ret NoMore %u",
 (unsigned int)profileLevelType->nProfileIndex);
                eRet = OMX_ErrorNoMore;
 }
 } else if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.hevc", OMX_MAX_STRINGNAME_SIZE)) {
 if (profileLevelType->nProfileIndex == 0) {
                profileLevelType->eProfile = OMX_VIDEO_HEVCProfileMain;
                profileLevelType->eLevel   = OMX_VIDEO_HEVCMainTierLevel51;
 } else {
                DEBUG_PRINT_LOW("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported nProfileIndex ret NoMore %u",
 (unsigned int)profileLevelType->nProfileIndex);
                eRet = OMX_ErrorNoMore;
 }
 } else if ((!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.h263",OMX_MAX_STRINGNAME_SIZE))) {
 if (profileLevelType->nProfileIndex == 0) {
                profileLevelType->eProfile = OMX_VIDEO_H263ProfileBaseline;
                profileLevelType->eLevel   = OMX_VIDEO_H263Level70;
 } else {
                DEBUG_PRINT_LOW("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported nProfileIndex ret NoMore %u",
 (unsigned int)profileLevelType->nProfileIndex);
                eRet = OMX_ErrorNoMore;
 }
 } else if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.mpeg4",OMX_MAX_STRINGNAME_SIZE)) {
 if (profileLevelType->nProfileIndex == 0) {
                profileLevelType->eProfile = OMX_VIDEO_MPEG4ProfileSimple;
                profileLevelType->eLevel   = OMX_VIDEO_MPEG4Level5;
 } else if (profileLevelType->nProfileIndex == 1) {
                profileLevelType->eProfile = OMX_VIDEO_MPEG4ProfileAdvancedSimple;
                profileLevelType->eLevel   = OMX_VIDEO_MPEG4Level5;
 } else {
                DEBUG_PRINT_LOW("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported nProfileIndex ret NoMore %u",
 (unsigned int)profileLevelType->nProfileIndex);
                eRet = OMX_ErrorNoMore;
 }
 } else if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.vp8",OMX_MAX_STRINGNAME_SIZE)) {
            eRet = OMX_ErrorNoMore;
 } else if (!strncmp(drv_ctx.kind, "OMX.qcom.video.decoder.mpeg2",OMX_MAX_STRINGNAME_SIZE)) {
 if (profileLevelType->nProfileIndex == 0) {
                profileLevelType->eProfile = OMX_VIDEO_MPEG2ProfileSimple;
                profileLevelType->eLevel   = OMX_VIDEO_MPEG2LevelHL;
 } else if (profileLevelType->nProfileIndex == 1) {
                profileLevelType->eProfile = OMX_VIDEO_MPEG2ProfileMain;
                profileLevelType->eLevel   = OMX_VIDEO_MPEG2LevelHL;
 } else {
                DEBUG_PRINT_LOW("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported nProfileIndex ret NoMore %u",
 (unsigned int)profileLevelType->nProfileIndex);
                eRet = OMX_ErrorNoMore;
 }
 } else {
            DEBUG_PRINT_ERROR("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported ret NoMore for codec: %s", drv_ctx.kind);
            eRet = OMX_ErrorNoMore;
 }
 } else {
        DEBUG_PRINT_ERROR("get_parameter: OMX_IndexParamVideoProfileLevelQuerySupported should be queries on Input port only %u",
 (unsigned int)profileLevelType->nPortIndex);
        eRet = OMX_ErrorBadPortIndex;
 }
 return eRet;
}
