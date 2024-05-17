OMX_ERRORTYPE  omx_vdec::get_extension_index(OMX_IN OMX_HANDLETYPE      hComp,
        OMX_IN OMX_STRING      paramName,
        OMX_OUT OMX_INDEXTYPE* indexType)
{
 (void) hComp;
 if (m_state == OMX_StateInvalid) {
        DEBUG_PRINT_ERROR("Get Extension Index in Invalid State");
 return OMX_ErrorInvalidState;
 } else if (extn_equals(paramName, "OMX.QCOM.index.param.video.SyncFrameDecodingMode")) {
 *indexType = (OMX_INDEXTYPE)OMX_QcomIndexParamVideoSyncFrameDecodingMode;
 } else if (extn_equals(paramName, "OMX.QCOM.index.param.IndexExtraData")) {
 *indexType = (OMX_INDEXTYPE)OMX_QcomIndexParamIndexExtraDataType;
 } else if (extn_equals(paramName, OMX_QCOM_INDEX_PARAM_VIDEO_FRAMEPACKING_EXTRADATA)) {
 *indexType = (OMX_INDEXTYPE)OMX_QcomIndexParamVideoFramePackingExtradata;
 } else if (extn_equals(paramName, OMX_QCOM_INDEX_CONFIG_VIDEO_FRAMEPACKING_INFO)) {
 *indexType = (OMX_INDEXTYPE)OMX_QcomIndexConfigVideoFramePackingArrangement;
 } else if (extn_equals(paramName, OMX_QCOM_INDEX_PARAM_VIDEO_QP_EXTRADATA)) {
 *indexType = (OMX_INDEXTYPE)OMX_QcomIndexParamVideoQPExtraData;
 } else if (extn_equals(paramName, OMX_QCOM_INDEX_PARAM_VIDEO_INPUTBITSINFO_EXTRADATA)) {
 *indexType = (OMX_INDEXTYPE)OMX_QcomIndexParamVideoInputBitsInfoExtraData;
 } else if (extn_equals(paramName, OMX_QCOM_INDEX_PARAM_VIDEO_EXTNUSER_EXTRADATA)) {
 *indexType = (OMX_INDEXTYPE)OMX_QcomIndexEnableExtnUserData;
 } else if (extn_equals(paramName, OMX_QCOM_INDEX_PARAM_VIDEO_MPEG2SEQDISP_EXTRADATA)) {
 *indexType = (OMX_INDEXTYPE)OMX_QcomIndexParamMpeg2SeqDispExtraData;
 }
#if defined (_ANDROID_HONEYCOMB_) || defined (_ANDROID_ICS_)
 else if (extn_equals(paramName, "OMX.google.android.index.enableAndroidNativeBuffers")) {
 *indexType = (OMX_INDEXTYPE)OMX_GoogleAndroidIndexEnableAndroidNativeBuffers;
 } else if (extn_equals(paramName, "OMX.google.android.index.useAndroidNativeBuffer2")) {
 *indexType = (OMX_INDEXTYPE)OMX_GoogleAndroidIndexUseAndroidNativeBuffer2;
 } else if (extn_equals(paramName, "OMX.google.android.index.useAndroidNativeBuffer")) {
        DEBUG_PRINT_ERROR("Extension: %s is supported", paramName);
 *indexType = (OMX_INDEXTYPE)OMX_GoogleAndroidIndexUseAndroidNativeBuffer;
 } else if (extn_equals(paramName, "OMX.google.android.index.getAndroidNativeBufferUsage")) {
 *indexType = (OMX_INDEXTYPE)OMX_GoogleAndroidIndexGetAndroidNativeBufferUsage;
 }
#endif
 else if (extn_equals(paramName, "OMX.google.android.index.storeMetaDataInBuffers")) {
 *indexType = (OMX_INDEXTYPE)OMX_QcomIndexParamVideoMetaBufferMode;
 }
#ifdef ADAPTIVE_PLAYBACK_SUPPORTED
 else if (extn_equals(paramName, "OMX.google.android.index.prepareForAdaptivePlayback")) {
 *indexType = (OMX_INDEXTYPE)OMX_QcomIndexParamVideoAdaptivePlaybackMode;
 }
#endif
#ifdef FLEXYUV_SUPPORTED
 else if (extn_equals(paramName,"OMX.google.android.index.describeColorFormat")) {
 *indexType = (OMX_INDEXTYPE)OMX_QcomIndexFlexibleYUVDescription;
 }
#endif
 else {
        DEBUG_PRINT_ERROR("Extension: %s not implemented", paramName);
 return OMX_ErrorNotImplemented;
 }
 return OMX_ErrorNone;
}
