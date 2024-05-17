OMX_ERRORTYPE SoftVPXEncoder::internalSetProfileLevel(
 const OMX_VIDEO_PARAM_PROFILELEVELTYPE* profileAndLevel) {

 if (!isValidOMXParam(profileAndLevel)) {
 return OMX_ErrorBadParameter;
 }

 if (profileAndLevel->nPortIndex != kOutputPortIndex) {
 return OMX_ErrorUnsupportedIndex;
 }

 if (profileAndLevel->eProfile != OMX_VIDEO_VP8ProfileMain) {
 return OMX_ErrorBadParameter;
 }

 if (profileAndLevel->eLevel == OMX_VIDEO_VP8Level_Version0 ||
        profileAndLevel->eLevel == OMX_VIDEO_VP8Level_Version1 ||
        profileAndLevel->eLevel == OMX_VIDEO_VP8Level_Version2 ||
        profileAndLevel->eLevel == OMX_VIDEO_VP8Level_Version3) {
        mLevel = (OMX_VIDEO_VP8LEVELTYPE)profileAndLevel->eLevel;
 } else {
 return OMX_ErrorBadParameter;
 }

 return OMX_ErrorNone;
}
