OMX_ERRORTYPE SoftVPXEncoder::internalGetParameter(OMX_INDEXTYPE index,
                                                   OMX_PTR param) {
 const int32_t indexFull = index;

 switch (indexFull) {
 case OMX_IndexParamVideoPortFormat: {
            OMX_VIDEO_PARAM_PORTFORMATTYPE *formatParams =
 (OMX_VIDEO_PARAM_PORTFORMATTYPE *)param;

 if (!isValidOMXParam(formatParams)) {
 return OMX_ErrorBadParameter;
 }

 if (formatParams->nPortIndex == kInputPortIndex) {
 if (formatParams->nIndex >= kNumberOfSupportedColorFormats) {
 return OMX_ErrorNoMore;
 }

 if (formatParams->nIndex == 0) {
                    formatParams->eColorFormat = OMX_COLOR_FormatYUV420Planar;
 } else if (formatParams->nIndex == 1) {
                    formatParams->eColorFormat =
                        OMX_COLOR_FormatYUV420SemiPlanar;
 } else {
                    formatParams->eColorFormat = OMX_COLOR_FormatAndroidOpaque;
 }

                formatParams->eCompressionFormat = OMX_VIDEO_CodingUnused;
                formatParams->xFramerate = mFramerate;
 return OMX_ErrorNone;
 } else if (formatParams->nPortIndex == kOutputPortIndex) {
                formatParams->eCompressionFormat = OMX_VIDEO_CodingVP8;
                formatParams->eColorFormat = OMX_COLOR_FormatUnused;
                formatParams->xFramerate = 0;
 return OMX_ErrorNone;
 } else {
 return OMX_ErrorBadPortIndex;
 }
 }

 case OMX_IndexParamVideoBitrate: {
            OMX_VIDEO_PARAM_BITRATETYPE *bitrate =
 (OMX_VIDEO_PARAM_BITRATETYPE *)param;

 if (!isValidOMXParam(bitrate)) {
 return OMX_ErrorBadParameter;
 }

 if (bitrate->nPortIndex != kOutputPortIndex) {
 return OMX_ErrorUnsupportedIndex;
 }

            bitrate->nTargetBitrate = mBitrate;

 if (mBitrateControlMode == VPX_VBR) {
                bitrate->eControlRate = OMX_Video_ControlRateVariable;
 } else if (mBitrateControlMode == VPX_CBR) {
                bitrate->eControlRate = OMX_Video_ControlRateConstant;
 } else {
 return OMX_ErrorUnsupportedSetting;
 }
 return OMX_ErrorNone;
 }

 case OMX_IndexParamVideoVp8: {
            OMX_VIDEO_PARAM_VP8TYPE *vp8Params =
 (OMX_VIDEO_PARAM_VP8TYPE *)param;

 if (!isValidOMXParam(vp8Params)) {
 return OMX_ErrorBadParameter;
 }

 if (vp8Params->nPortIndex != kOutputPortIndex) {
 return OMX_ErrorUnsupportedIndex;
 }

            vp8Params->eProfile = OMX_VIDEO_VP8ProfileMain;
            vp8Params->eLevel = mLevel;
            vp8Params->nDCTPartitions = mDCTPartitions;
            vp8Params->bErrorResilientMode = mErrorResilience;
 return OMX_ErrorNone;
 }

 case OMX_IndexParamVideoAndroidVp8Encoder: {
            OMX_VIDEO_PARAM_ANDROID_VP8ENCODERTYPE *vp8AndroidParams =
 (OMX_VIDEO_PARAM_ANDROID_VP8ENCODERTYPE *)param;

 if (!isValidOMXParam(vp8AndroidParams)) {
 return OMX_ErrorBadParameter;
 }

 if (vp8AndroidParams->nPortIndex != kOutputPortIndex) {
 return OMX_ErrorUnsupportedIndex;
 }

                vp8AndroidParams->nKeyFrameInterval = mKeyFrameInterval;
                vp8AndroidParams->eTemporalPattern = mTemporalPatternType;
                vp8AndroidParams->nTemporalLayerCount = mTemporalLayers;
                vp8AndroidParams->nMinQuantizer = mMinQuantizer;
                vp8AndroidParams->nMaxQuantizer = mMaxQuantizer;
                memcpy(vp8AndroidParams->nTemporalLayerBitrateRatio,
                       mTemporalLayerBitrateRatio, sizeof(mTemporalLayerBitrateRatio));
 return OMX_ErrorNone;
 }

 case OMX_IndexParamVideoProfileLevelQuerySupported: {
            OMX_VIDEO_PARAM_PROFILELEVELTYPE *profileAndLevel =
 (OMX_VIDEO_PARAM_PROFILELEVELTYPE *)param;

 if (!isValidOMXParam(profileAndLevel)) {
 return OMX_ErrorBadParameter;
 }

 if (profileAndLevel->nPortIndex != kOutputPortIndex) {
 return OMX_ErrorUnsupportedIndex;
 }

 switch (profileAndLevel->nProfileIndex) {
 case 0:
                    profileAndLevel->eLevel = OMX_VIDEO_VP8Level_Version0;
 break;

 case 1:
                    profileAndLevel->eLevel = OMX_VIDEO_VP8Level_Version1;
 break;

 case 2:
                    profileAndLevel->eLevel = OMX_VIDEO_VP8Level_Version2;
 break;

 case 3:
                    profileAndLevel->eLevel = OMX_VIDEO_VP8Level_Version3;
 break;

 default:
 return OMX_ErrorNoMore;
 }

            profileAndLevel->eProfile = OMX_VIDEO_VP8ProfileMain;
 return OMX_ErrorNone;
 }

 case OMX_IndexParamVideoProfileLevelCurrent: {
            OMX_VIDEO_PARAM_PROFILELEVELTYPE *profileAndLevel =
 (OMX_VIDEO_PARAM_PROFILELEVELTYPE *)param;

 if (!isValidOMXParam(profileAndLevel)) {
 return OMX_ErrorBadParameter;
 }

 if (profileAndLevel->nPortIndex != kOutputPortIndex) {
 return OMX_ErrorUnsupportedIndex;
 }

            profileAndLevel->eLevel = mLevel;
            profileAndLevel->eProfile = OMX_VIDEO_VP8ProfileMain;
 return OMX_ErrorNone;
 }

 default:
 return SimpleSoftOMXComponent::internalGetParameter(index, param);
 }
}
