status_t ACodec::setupHEVCEncoderParameters(const sp<AMessage> &msg) {
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

    OMX_VIDEO_PARAM_HEVCTYPE hevcType;
 InitOMXParams(&hevcType);
    hevcType.nPortIndex = kPortIndexOutput;

 status_t err = OK;
    err = mOMX->getParameter(
            mNode, (OMX_INDEXTYPE)OMX_IndexParamVideoHevc, &hevcType, sizeof(hevcType));
 if (err != OK) {
 return err;
 }

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

        hevcType.eProfile = static_cast<OMX_VIDEO_HEVCPROFILETYPE>(profile);
        hevcType.eLevel = static_cast<OMX_VIDEO_HEVCLEVELTYPE>(level);
 }


    err = mOMX->setParameter(
            mNode, (OMX_INDEXTYPE)OMX_IndexParamVideoHevc, &hevcType, sizeof(hevcType));
 if (err != OK) {
 return err;
 }

 return configureBitrate(bitrate, bitrateMode);
}
