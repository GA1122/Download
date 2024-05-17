OMX_ERRORTYPE SoftMPEG4Encoder::internalGetParameter(
        OMX_INDEXTYPE index, OMX_PTR params) {
 switch (index) {
 case OMX_IndexParamVideoErrorCorrection:
 {
 return OMX_ErrorNotImplemented;
 }

 case OMX_IndexParamVideoBitrate:
 {
            OMX_VIDEO_PARAM_BITRATETYPE *bitRate =
 (OMX_VIDEO_PARAM_BITRATETYPE *) params;

 if (!isValidOMXParam(bitRate)) {
 return OMX_ErrorBadParameter;
 }

 if (bitRate->nPortIndex != 1) {
 return OMX_ErrorUndefined;
 }

            bitRate->eControlRate = OMX_Video_ControlRateVariable;
            bitRate->nTargetBitrate = mVideoBitRate;
 return OMX_ErrorNone;
 }

 case OMX_IndexParamVideoPortFormat:
 {
            OMX_VIDEO_PARAM_PORTFORMATTYPE *formatParams =
 (OMX_VIDEO_PARAM_PORTFORMATTYPE *)params;

 if (!isValidOMXParam(formatParams)) {
 return OMX_ErrorBadParameter;
 }

 if (formatParams->nPortIndex > 1) {
 return OMX_ErrorUndefined;
 }

 if (formatParams->nIndex > 2) {
 return OMX_ErrorNoMore;
 }

 if (formatParams->nPortIndex == 0) {
                formatParams->eCompressionFormat = OMX_VIDEO_CodingUnused;
 if (formatParams->nIndex == 0) {
                    formatParams->eColorFormat = OMX_COLOR_FormatYUV420Planar;
 } else if (formatParams->nIndex == 1) {
                    formatParams->eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
 } else {
                    formatParams->eColorFormat = OMX_COLOR_FormatAndroidOpaque;
 }
 } else {
                formatParams->eCompressionFormat =
 (mEncodeMode == COMBINE_MODE_WITH_ERR_RES)
 ? OMX_VIDEO_CodingMPEG4
 : OMX_VIDEO_CodingH263;

                formatParams->eColorFormat = OMX_COLOR_FormatUnused;
 }

 return OMX_ErrorNone;
 }

 case OMX_IndexParamVideoH263:
 {
            OMX_VIDEO_PARAM_H263TYPE *h263type =
 (OMX_VIDEO_PARAM_H263TYPE *)params;

 if (!isValidOMXParam(h263type)) {
 return OMX_ErrorBadParameter;
 }

 if (h263type->nPortIndex != 1) {
 return OMX_ErrorUndefined;
 }

            h263type->nAllowedPictureTypes =
 (OMX_VIDEO_PictureTypeI | OMX_VIDEO_PictureTypeP);
            h263type->eProfile = OMX_VIDEO_H263ProfileBaseline;
            h263type->eLevel = OMX_VIDEO_H263Level45;
            h263type->bPLUSPTYPEAllowed = OMX_FALSE;
            h263type->bForceRoundingTypeToZero = OMX_FALSE;
            h263type->nPictureHeaderRepetition = 0;
            h263type->nGOBHeaderInterval = 0;

 return OMX_ErrorNone;
 }

 case OMX_IndexParamVideoMpeg4:
 {
            OMX_VIDEO_PARAM_MPEG4TYPE *mpeg4type =
 (OMX_VIDEO_PARAM_MPEG4TYPE *)params;

 if (!isValidOMXParam(mpeg4type)) {
 return OMX_ErrorBadParameter;
 }

 if (mpeg4type->nPortIndex != 1) {
 return OMX_ErrorUndefined;
 }

            mpeg4type->eProfile = OMX_VIDEO_MPEG4ProfileCore;
            mpeg4type->eLevel = OMX_VIDEO_MPEG4Level2;
            mpeg4type->nAllowedPictureTypes =
 (OMX_VIDEO_PictureTypeI | OMX_VIDEO_PictureTypeP);
            mpeg4type->nBFrames = 0;
            mpeg4type->nIDCVLCThreshold = 0;
            mpeg4type->bACPred = OMX_TRUE;
            mpeg4type->nMaxPacketSize = 256;
            mpeg4type->nTimeIncRes = 1000;
            mpeg4type->nHeaderExtension = 0;
            mpeg4type->bReversibleVLC = OMX_FALSE;

 return OMX_ErrorNone;
 }

 case OMX_IndexParamVideoProfileLevelQuerySupported:
 {
            OMX_VIDEO_PARAM_PROFILELEVELTYPE *profileLevel =
 (OMX_VIDEO_PARAM_PROFILELEVELTYPE *)params;

 if (!isValidOMXParam(profileLevel)) {
 return OMX_ErrorBadParameter;
 }

 if (profileLevel->nPortIndex != 1) {
 return OMX_ErrorUndefined;
 }

 if (profileLevel->nProfileIndex > 0) {
 return OMX_ErrorNoMore;
 }

 if (mEncodeMode == H263_MODE) {
                profileLevel->eProfile = OMX_VIDEO_H263ProfileBaseline;
                profileLevel->eLevel = OMX_VIDEO_H263Level45;
 } else {
                profileLevel->eProfile = OMX_VIDEO_MPEG4ProfileCore;
                profileLevel->eLevel = OMX_VIDEO_MPEG4Level2;
 }

 return OMX_ErrorNone;
 }

 default:
 return SimpleSoftOMXComponent::internalGetParameter(index, params);
 }
}
