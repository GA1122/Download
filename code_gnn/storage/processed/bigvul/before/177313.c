status_t ACodec::setupAVCEncoderParameters(const sp<AMessage> &msg) {
 int32_t bitrate, iFrameInterval;
 if (!msg->findInt32("bitrate", &bitrate)
 || !msg->findInt32("i-frame-interval", &iFrameInterval)) {
 return INVALID_OPERATION;
 }

    OMX_VIDEO_CONTROLRATETYPE bitrateMode = getBitrateMode(msg);

 float frameRate;
 if (!msg->findFloat("frame-rate", &frameRate)) {
 int32_t tmp;
 if (!msg->findInt32("frame-rate", &tmp)) {
 return INVALID_OPERATION;
 }
        frameRate = (float)tmp;
 }

 status_t err = OK;
 int32_t intraRefreshMode = 0;
 if (msg->findInt32("intra-refresh-mode", &intraRefreshMode)) {
        err = setCyclicIntraMacroblockRefresh(msg, intraRefreshMode);
 if (err != OK) {
            ALOGE("Setting intra macroblock refresh mode (%d) failed: 0x%x",
                    err, intraRefreshMode);
 return err;
 }
 }

    OMX_VIDEO_PARAM_AVCTYPE h264type;
 InitOMXParams(&h264type);
    h264type.nPortIndex = kPortIndexOutput;

    err = mOMX->getParameter(
            mNode, OMX_IndexParamVideoAvc, &h264type, sizeof(h264type));

 if (err != OK) {
 return err;
 }

    h264type.nAllowedPictureTypes =
        OMX_VIDEO_PictureTypeI | OMX_VIDEO_PictureTypeP;

 int32_t profile;
 if (msg->findInt32("profile", &profile)) {
 int32_t level;
 if (!msg->findInt32("level", &level)) {
 return INVALID_OPERATION;
 }

        err = verifySupportForProfileAndLevel(profile, level);

 if (err != OK) {
 return err;
 }

        h264type.eProfile = static_cast<OMX_VIDEO_AVCPROFILETYPE>(profile);
        h264type.eLevel = static_cast<OMX_VIDEO_AVCLEVELTYPE>(level);
 }

 if (h264type.eProfile != OMX_VIDEO_AVCProfileBaseline) {
        ALOGW("Use baseline profile instead of %d for AVC recording",
            h264type.eProfile);
        h264type.eProfile = OMX_VIDEO_AVCProfileBaseline;
 }

 if (h264type.eProfile == OMX_VIDEO_AVCProfileBaseline) {
        h264type.nSliceHeaderSpacing = 0;
        h264type.bUseHadamard = OMX_TRUE;
        h264type.nRefFrames = 1;
        h264type.nBFrames = 0;
        h264type.nPFrames = setPFramesSpacing(iFrameInterval, frameRate);
 if (h264type.nPFrames == 0) {
            h264type.nAllowedPictureTypes = OMX_VIDEO_PictureTypeI;
 }
        h264type.nRefIdx10ActiveMinus1 = 0;
        h264type.nRefIdx11ActiveMinus1 = 0;
        h264type.bEntropyCodingCABAC = OMX_FALSE;
        h264type.bWeightedPPrediction = OMX_FALSE;
        h264type.bconstIpred = OMX_FALSE;
        h264type.bDirect8x8Inference = OMX_FALSE;
        h264type.bDirectSpatialTemporal = OMX_FALSE;
        h264type.nCabacInitIdc = 0;
 }

 if (h264type.nBFrames != 0) {
        h264type.nAllowedPictureTypes |= OMX_VIDEO_PictureTypeB;
 }

    h264type.bEnableUEP = OMX_FALSE;
    h264type.bEnableFMO = OMX_FALSE;
    h264type.bEnableASO = OMX_FALSE;
    h264type.bEnableRS = OMX_FALSE;
    h264type.bFrameMBsOnly = OMX_TRUE;
    h264type.bMBAFF = OMX_FALSE;
    h264type.eLoopFilterMode = OMX_VIDEO_AVCLoopFilterEnable;

    err = mOMX->setParameter(
            mNode, OMX_IndexParamVideoAvc, &h264type, sizeof(h264type));

 if (err != OK) {
 return err;
 }

 return configureBitrate(bitrate, bitrateMode);
}
