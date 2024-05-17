OMX_ERRORTYPE SoftMPEG4Encoder::internalSetParameter(
        OMX_INDEXTYPE index, const OMX_PTR params) {
 int32_t indexFull = index;

 switch (indexFull) {
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

 if (bitRate->nPortIndex != 1 ||
                bitRate->eControlRate != OMX_Video_ControlRateVariable) {
 return OMX_ErrorUndefined;
 }

            mVideoBitRate = bitRate->nTargetBitrate;
 return OMX_ErrorNone;
 }

 case OMX_IndexParamPortDefinition:
 {
            OMX_PARAM_PORTDEFINITIONTYPE *def =
 (OMX_PARAM_PORTDEFINITIONTYPE *)params;

 if (!isValidOMXParam(def)) {
 return OMX_ErrorBadParameter;
 }

 if (def->nPortIndex > 1) {
 return OMX_ErrorUndefined;
 }

 if (def->nPortIndex == 0) {
 if (def->format.video.eCompressionFormat != OMX_VIDEO_CodingUnused ||
 (def->format.video.eColorFormat != OMX_COLOR_FormatYUV420Planar &&
                     def->format.video.eColorFormat != OMX_COLOR_FormatYUV420SemiPlanar &&
                     def->format.video.eColorFormat != OMX_COLOR_FormatAndroidOpaque)) {
 return OMX_ErrorUndefined;
 }
 } else {
 if ((mEncodeMode == COMBINE_MODE_WITH_ERR_RES &&
                        def->format.video.eCompressionFormat != OMX_VIDEO_CodingMPEG4) ||
 (mEncodeMode == H263_MODE &&
                        def->format.video.eCompressionFormat != OMX_VIDEO_CodingH263) ||
 (def->format.video.eColorFormat != OMX_COLOR_FormatUnused)) {
 return OMX_ErrorUndefined;
 }
 }

            OMX_ERRORTYPE err = SimpleSoftOMXComponent::internalSetParameter(index, params);
 if (OMX_ErrorNone != err) {
 return err;
 }

 if (def->nPortIndex == 0) {
                mVideoWidth = def->format.video.nFrameWidth;
                mVideoHeight = def->format.video.nFrameHeight;
                mVideoFrameRate = def->format.video.xFramerate >> 16;
                mVideoColorFormat = def->format.video.eColorFormat;

                OMX_PARAM_PORTDEFINITIONTYPE *portDef =
 &editPortInfo(0)->mDef;
                portDef->format.video.nFrameWidth = mVideoWidth;
                portDef->format.video.nFrameHeight = mVideoHeight;
                portDef->format.video.xFramerate = def->format.video.xFramerate;
                portDef->format.video.eColorFormat =
 (OMX_COLOR_FORMATTYPE) mVideoColorFormat;
                portDef = &editPortInfo(1)->mDef;
                portDef->format.video.nFrameWidth = mVideoWidth;
                portDef->format.video.nFrameHeight = mVideoHeight;
 } else {
                mVideoBitRate = def->format.video.nBitrate;
 }

 return OMX_ErrorNone;
 }

 case OMX_IndexParamStandardComponentRole:
 {
 const OMX_PARAM_COMPONENTROLETYPE *roleParams =
 (const OMX_PARAM_COMPONENTROLETYPE *)params;

 if (!isValidOMXParam(roleParams)) {
 return OMX_ErrorBadParameter;
 }

 if (strncmp((const char *)roleParams->cRole,
 (mEncodeMode == H263_MODE)
 ? "video_encoder.h263": "video_encoder.mpeg4",
                        OMX_MAX_STRINGNAME_SIZE - 1)) {
 return OMX_ErrorUndefined;
 }

 return OMX_ErrorNone;
 }

 case OMX_IndexParamVideoPortFormat:
 {
 const OMX_VIDEO_PARAM_PORTFORMATTYPE *formatParams =
 (const OMX_VIDEO_PARAM_PORTFORMATTYPE *)params;

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
 if (formatParams->eCompressionFormat != OMX_VIDEO_CodingUnused ||
 ((formatParams->nIndex == 0 &&
                      formatParams->eColorFormat != OMX_COLOR_FormatYUV420Planar) ||
 (formatParams->nIndex == 1 &&
                     formatParams->eColorFormat != OMX_COLOR_FormatYUV420SemiPlanar) ||
 (formatParams->nIndex == 2 &&
                     formatParams->eColorFormat != OMX_COLOR_FormatAndroidOpaque) )) {
 return OMX_ErrorUndefined;
 }
                mVideoColorFormat = formatParams->eColorFormat;
 } else {
 if ((mEncodeMode == H263_MODE &&
                        formatParams->eCompressionFormat != OMX_VIDEO_CodingH263) ||
 (mEncodeMode == COMBINE_MODE_WITH_ERR_RES &&
                        formatParams->eCompressionFormat != OMX_VIDEO_CodingMPEG4) ||
                    formatParams->eColorFormat != OMX_COLOR_FormatUnused) {
 return OMX_ErrorUndefined;
 }
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

 if (h263type->eProfile != OMX_VIDEO_H263ProfileBaseline ||
                h263type->eLevel != OMX_VIDEO_H263Level45 ||
 (h263type->nAllowedPictureTypes & OMX_VIDEO_PictureTypeB) ||
                h263type->bPLUSPTYPEAllowed != OMX_FALSE ||
                h263type->bForceRoundingTypeToZero != OMX_FALSE ||
                h263type->nPictureHeaderRepetition != 0 ||
                h263type->nGOBHeaderInterval != 0) {
 return OMX_ErrorUndefined;
 }

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

 if (mpeg4type->eProfile != OMX_VIDEO_MPEG4ProfileCore ||
                mpeg4type->eLevel != OMX_VIDEO_MPEG4Level2 ||
 (mpeg4type->nAllowedPictureTypes & OMX_VIDEO_PictureTypeB) ||
                mpeg4type->nBFrames != 0 ||
                mpeg4type->nIDCVLCThreshold != 0 ||
                mpeg4type->bACPred != OMX_TRUE ||
                mpeg4type->nMaxPacketSize != 256 ||
                mpeg4type->nTimeIncRes != 1000 ||
                mpeg4type->nHeaderExtension != 0 ||
                mpeg4type->bReversibleVLC != OMX_FALSE) {
 return OMX_ErrorUndefined;
 }

 return OMX_ErrorNone;
 }

 case kStoreMetaDataExtensionIndex:
 {
 StoreMetaDataInBuffersParams *storeParams =
 (StoreMetaDataInBuffersParams*)params;

 if (!isValidOMXParam(storeParams)) {
 return OMX_ErrorBadParameter;
 }

 if (storeParams->nPortIndex != 0) {
                ALOGE("%s: StoreMetadataInBuffersParams.nPortIndex not zero!",
                        __FUNCTION__);
 return OMX_ErrorUndefined;
 }

            mStoreMetaDataInBuffers = storeParams->bStoreMetaData;
            ALOGV("StoreMetaDataInBuffers set to: %s",
                    mStoreMetaDataInBuffers ? " true" : "false");

 if (mStoreMetaDataInBuffers) {
                mVideoColorFormat = OMX_COLOR_FormatAndroidOpaque;
 }

 return OMX_ErrorNone;
 }

 default:
 return SimpleSoftOMXComponent::internalSetParameter(index, params);
 }
}
