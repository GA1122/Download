OMX_ERRORTYPE SoftVPXEncoder::internalSetParameter(OMX_INDEXTYPE index,
 const OMX_PTR param) {
 const int32_t indexFull = index;

 switch (indexFull) {
 case OMX_IndexParamStandardComponentRole: {
 const OMX_PARAM_COMPONENTROLETYPE *role =
 (const OMX_PARAM_COMPONENTROLETYPE*) param;

 if (!isValidOMXParam(role)) {
 return OMX_ErrorBadParameter;
 }
 return internalSetRoleParams(role);
 }

 case OMX_IndexParamVideoBitrate: {
 const OMX_VIDEO_PARAM_BITRATETYPE *bitRate =
 (const OMX_VIDEO_PARAM_BITRATETYPE*) param;

 if (!isValidOMXParam(bitRate)) {
 return OMX_ErrorBadParameter;
 }

 return internalSetBitrateParams(bitRate);
 }

 case OMX_IndexParamPortDefinition:
 {
 const OMX_PARAM_PORTDEFINITIONTYPE *portDefinition =
 (const OMX_PARAM_PORTDEFINITIONTYPE*) param;

 if (!isValidOMXParam(portDefinition)) {
 return OMX_ErrorBadParameter;
 }

            OMX_ERRORTYPE err = internalSetPortParams(portDefinition);

 if (err != OMX_ErrorNone) {
 return err;
 }

 return SimpleSoftOMXComponent::internalSetParameter(index, param);
 }

 case OMX_IndexParamVideoPortFormat: {
 const OMX_VIDEO_PARAM_PORTFORMATTYPE *portFormatType =
 (const OMX_VIDEO_PARAM_PORTFORMATTYPE*) param;

 if (!isValidOMXParam(portFormatType)) {
 return OMX_ErrorBadParameter;
 }
 return internalSetFormatParams(portFormatType);
 }

 case OMX_IndexParamVideoVp8: {
 const OMX_VIDEO_PARAM_VP8TYPE *vp8Params =
 (const OMX_VIDEO_PARAM_VP8TYPE*) param;

 if (!isValidOMXParam(vp8Params)) {
 return OMX_ErrorBadParameter;
 }

 return internalSetVp8Params(vp8Params);
 }

 case OMX_IndexParamVideoProfileLevelCurrent: {
 const OMX_VIDEO_PARAM_PROFILELEVELTYPE *vp8Params =
 (const OMX_VIDEO_PARAM_PROFILELEVELTYPE*) param;

 if (!isValidOMXParam(vp8Params)) {
 return OMX_ErrorBadParameter;
 }

 return internalSetProfileLevel(vp8Params);
 }

 case OMX_IndexParamVideoAndroidVp8Encoder:
 return internalSetAndroidVp8Params(
 (const OMX_VIDEO_PARAM_ANDROID_VP8ENCODERTYPE *)param);

 case kStoreMetaDataExtensionIndex:
 {
 const StoreMetaDataInBuffersParams *storeParam =
 (const StoreMetaDataInBuffersParams *)param;

 if (!isValidOMXParam(storeParam)) {
 return OMX_ErrorBadParameter;
 }

 if (storeParam->nPortIndex != kInputPortIndex) {
 return OMX_ErrorBadPortIndex;
 }

            mInputDataIsMeta = (storeParam->bStoreMetaData == OMX_TRUE);

 return OMX_ErrorNone;
 }

 default:
 return SimpleSoftOMXComponent::internalSetParameter(index, param);
 }
}
