status_t ACodec::setupMPEG4EncoderParameters(const sp<AMessage> &msg) {
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

    OMX_VIDEO_PARAM_MPEG4TYPE mpeg4type;
 InitOMXParams(&mpeg4type);
    mpeg4type.nPortIndex = kPortIndexOutput;

 status_t err = mOMX->getParameter(
            mNode, OMX_IndexParamVideoMpeg4, &mpeg4type, sizeof(mpeg4type));

 if (err != OK) {
 return err;
 }

    mpeg4type.nSliceHeaderSpacing = 0;
    mpeg4type.bSVH = OMX_FALSE;
    mpeg4type.bGov = OMX_FALSE;

    mpeg4type.nAllowedPictureTypes =
        OMX_VIDEO_PictureTypeI | OMX_VIDEO_PictureTypeP;

    mpeg4type.nPFrames = setPFramesSpacing(iFrameInterval, frameRate);
 if (mpeg4type.nPFrames == 0) {
        mpeg4type.nAllowedPictureTypes = OMX_VIDEO_PictureTypeI;
 }
    mpeg4type.nBFrames = 0;
    mpeg4type.nIDCVLCThreshold = 0;
    mpeg4type.bACPred = OMX_TRUE;
    mpeg4type.nMaxPacketSize = 256;
    mpeg4type.nTimeIncRes = 1000;
    mpeg4type.nHeaderExtension = 0;
    mpeg4type.bReversibleVLC = OMX_FALSE;

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

        mpeg4type.eProfile = static_cast<OMX_VIDEO_MPEG4PROFILETYPE>(profile);
        mpeg4type.eLevel = static_cast<OMX_VIDEO_MPEG4LEVELTYPE>(level);
 }

    err = mOMX->setParameter(
            mNode, OMX_IndexParamVideoMpeg4, &mpeg4type, sizeof(mpeg4type));

 if (err != OK) {
 return err;
 }

    err = configureBitrate(bitrate, bitrateMode);

 if (err != OK) {
 return err;
 }

 return setupErrorCorrectionParameters();
}
