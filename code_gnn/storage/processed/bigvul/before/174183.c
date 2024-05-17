OMX_ERRORTYPE SoftVPXEncoder::internalSetAndroidVp8Params(
 const OMX_VIDEO_PARAM_ANDROID_VP8ENCODERTYPE* vp8AndroidParams) {
 if (vp8AndroidParams->nPortIndex != kOutputPortIndex) {
 return OMX_ErrorUnsupportedIndex;
 }
 if (vp8AndroidParams->eTemporalPattern != OMX_VIDEO_VPXTemporalLayerPatternNone &&
        vp8AndroidParams->eTemporalPattern != OMX_VIDEO_VPXTemporalLayerPatternWebRTC) {
 return OMX_ErrorBadParameter;
 }
 if (vp8AndroidParams->nTemporalLayerCount > OMX_VIDEO_ANDROID_MAXVP8TEMPORALLAYERS) {
 return OMX_ErrorBadParameter;
 }
 if (vp8AndroidParams->nMinQuantizer > vp8AndroidParams->nMaxQuantizer) {
 return OMX_ErrorBadParameter;
 }

    mTemporalPatternType = vp8AndroidParams->eTemporalPattern;
 if (vp8AndroidParams->eTemporalPattern == OMX_VIDEO_VPXTemporalLayerPatternWebRTC) {
        mTemporalLayers = vp8AndroidParams->nTemporalLayerCount;
 } else if (vp8AndroidParams->eTemporalPattern == OMX_VIDEO_VPXTemporalLayerPatternNone) {
        mTemporalLayers = 0;
 }
 if (mTemporalLayers > 1) {
 for (size_t i = 0; i < mTemporalLayers - 1; i++) {
 if (vp8AndroidParams->nTemporalLayerBitrateRatio[i + 1] <=
                    vp8AndroidParams->nTemporalLayerBitrateRatio[i]) {
                ALOGE("Wrong bitrate ratio - should be in increasing order.");
 return OMX_ErrorBadParameter;
 }
 }
 }
    mKeyFrameInterval = vp8AndroidParams->nKeyFrameInterval;
    mMinQuantizer = vp8AndroidParams->nMinQuantizer;
    mMaxQuantizer = vp8AndroidParams->nMaxQuantizer;
    memcpy(mTemporalLayerBitrateRatio, vp8AndroidParams->nTemporalLayerBitrateRatio,
 sizeof(mTemporalLayerBitrateRatio));
    ALOGD("VP8: internalSetAndroidVp8Params. BRMode: %u. TS: %zu. KF: %u."
 " QP: %u - %u BR0: %u. BR1: %u. BR2: %u",
 (uint32_t)mBitrateControlMode, mTemporalLayers, mKeyFrameInterval,
          mMinQuantizer, mMaxQuantizer, mTemporalLayerBitrateRatio[0],
          mTemporalLayerBitrateRatio[1], mTemporalLayerBitrateRatio[2]);
 return OMX_ErrorNone;
}
