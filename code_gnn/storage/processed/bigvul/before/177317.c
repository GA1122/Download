status_t ACodec::setupH263EncoderParameters(const sp<AMessage> &msg) {
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

    OMX_VIDEO_PARAM_H263TYPE h263type;
 InitOMXParams(&h263type);
    h263type.nPortIndex = kPortIndexOutput;

 status_t err = mOMX->getParameter(
            mNode, OMX_IndexParamVideoH263, &h263type, sizeof(h263type));

 if (err != OK) {
 return err;
 }

    h263type.nAllowedPictureTypes =
        OMX_VIDEO_PictureTypeI | OMX_VIDEO_PictureTypeP;

    h263type.nPFrames = setPFramesSpacing(iFrameInterval, frameRate);
 if (h263type.nPFrames == 0) {
        h263type.nAllowedPictureTypes = OMX_VIDEO_PictureTypeI;
 }
    h263type.nBFrames = 0;

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

        h263type.eProfile = static_cast<OMX_VIDEO_H263PROFILETYPE>(profile);
        h263type.eLevel = static_cast<OMX_VIDEO_H263LEVELTYPE>(level);
 }

    h263type.bPLUSPTYPEAllowed = OMX_FALSE;
    h263type.bForceRoundingTypeToZero = OMX_FALSE;
    h263type.nPictureHeaderRepetition = 0;
    h263type.nGOBHeaderInterval = 0;

    err = mOMX->setParameter(
            mNode, OMX_IndexParamVideoH263, &h263type, sizeof(h263type));

 if (err != OK) {
 return err;
 }

    err = configureBitrate(bitrate, bitrateMode);

 if (err != OK) {
 return err;
 }

 return setupErrorCorrectionParameters();
}
