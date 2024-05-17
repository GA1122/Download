status_t ACodec::setupVPXEncoderParameters(const sp<AMessage> &msg) {
 int32_t bitrate;
 int32_t iFrameInterval = 0;
 size_t tsLayers = 0;
    OMX_VIDEO_ANDROID_VPXTEMPORALLAYERPATTERNTYPE pattern =
        OMX_VIDEO_VPXTemporalLayerPatternNone;
 static const uint32_t kVp8LayerRateAlloction
 [OMX_VIDEO_ANDROID_MAXVP8TEMPORALLAYERS]
 [OMX_VIDEO_ANDROID_MAXVP8TEMPORALLAYERS] = {
 {100, 100, 100},  
 { 60, 100, 100},  
 { 40, 60, 100},  
 };
 if (!msg->findInt32("bitrate", &bitrate)) {
 return INVALID_OPERATION;
 }
    msg->findInt32("i-frame-interval", &iFrameInterval);

    OMX_VIDEO_CONTROLRATETYPE bitrateMode = getBitrateMode(msg);

 float frameRate;
 if (!msg->findFloat("frame-rate", &frameRate)) {
 int32_t tmp;
 if (!msg->findInt32("frame-rate", &tmp)) {
 return INVALID_OPERATION;
 }
        frameRate = (float)tmp;
 }

 AString tsSchema;
 if (msg->findString("ts-schema", &tsSchema)) {
 if (tsSchema == "webrtc.vp8.1-layer") {
            pattern = OMX_VIDEO_VPXTemporalLayerPatternWebRTC;
            tsLayers = 1;
 } else if (tsSchema == "webrtc.vp8.2-layer") {
            pattern = OMX_VIDEO_VPXTemporalLayerPatternWebRTC;
            tsLayers = 2;
 } else if (tsSchema == "webrtc.vp8.3-layer") {
            pattern = OMX_VIDEO_VPXTemporalLayerPatternWebRTC;
            tsLayers = 3;
 } else {
            ALOGW("Unsupported ts-schema [%s]", tsSchema.c_str());
 }
 }

    OMX_VIDEO_PARAM_ANDROID_VP8ENCODERTYPE vp8type;
 InitOMXParams(&vp8type);
    vp8type.nPortIndex = kPortIndexOutput;
 status_t err = mOMX->getParameter(
            mNode, (OMX_INDEXTYPE)OMX_IndexParamVideoAndroidVp8Encoder,
 &vp8type, sizeof(vp8type));

 if (err == OK) {
 if (iFrameInterval > 0) {
            vp8type.nKeyFrameInterval = setPFramesSpacing(iFrameInterval, frameRate);
 }
        vp8type.eTemporalPattern = pattern;
        vp8type.nTemporalLayerCount = tsLayers;
 if (tsLayers > 0) {
 for (size_t i = 0; i < OMX_VIDEO_ANDROID_MAXVP8TEMPORALLAYERS; i++) {
                vp8type.nTemporalLayerBitrateRatio[i] =
                    kVp8LayerRateAlloction[tsLayers - 1][i];
 }
 }
 if (bitrateMode == OMX_Video_ControlRateConstant) {
            vp8type.nMinQuantizer = 2;
            vp8type.nMaxQuantizer = 63;
 }

        err = mOMX->setParameter(
                mNode, (OMX_INDEXTYPE)OMX_IndexParamVideoAndroidVp8Encoder,
 &vp8type, sizeof(vp8type));
 if (err != OK) {
            ALOGW("Extended VP8 parameters set failed: %d", err);
 }
 }

 return configureBitrate(bitrate, bitrateMode);
}
