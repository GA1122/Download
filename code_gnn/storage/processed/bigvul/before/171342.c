status_t OMXCodec::getVideoProfileLevel(
 const sp<MetaData>& meta,
 const CodecProfileLevel& defaultProfileLevel,
 CodecProfileLevel &profileLevel) {
    CODEC_LOGV("Default profile: %u, level #x%x",
            defaultProfileLevel.mProfile, defaultProfileLevel.mLevel);

 int32_t profile, level;
 if (!meta->findInt32(kKeyVideoProfile, &profile)) {
        profile = defaultProfileLevel.mProfile;
 }
 if (!meta->findInt32(kKeyVideoLevel, &level)) {
        level = defaultProfileLevel.mLevel;
 }
    CODEC_LOGV("Target profile: %d, level: %d", profile, level);

    OMX_VIDEO_PARAM_PROFILELEVELTYPE param;
 InitOMXParams(&param);
    param.nPortIndex = kPortIndexOutput;
 for (param.nProfileIndex = 0;; ++param.nProfileIndex) {
 status_t err = mOMX->getParameter(
                mNode, OMX_IndexParamVideoProfileLevelQuerySupported,
 &param, sizeof(param));

 if (err != OK) break;

 int32_t supportedProfile = static_cast<int32_t>(param.eProfile);
 int32_t supportedLevel = static_cast<int32_t>(param.eLevel);
        CODEC_LOGV("Supported profile: %d, level %d",
            supportedProfile, supportedLevel);

 if (profile == supportedProfile &&
            level <= supportedLevel) {
            profileLevel.mProfile = profile;
            profileLevel.mLevel = level;
 return OK;
 }
 }

    CODEC_LOGE("Target profile (%d) and level (%d) is not supported",
            profile, level);
 return BAD_VALUE;
}
