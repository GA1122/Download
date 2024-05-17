status_t SoftAACEncoder2::setAudioParams() {

    ALOGV("setAudioParams: %u Hz, %u channels, %u bps, %i sbr mode, %i sbr ratio",
         mSampleRate, mNumChannels, mBitRate, mSBRMode, mSBRRatio);

 if (AACENC_OK != aacEncoder_SetParam(mAACEncoder, AACENC_AOT,
            getAOTFromProfile(mAACProfile))) {
        ALOGE("Failed to set AAC encoder parameters");
 return UNKNOWN_ERROR;
 }

 if (AACENC_OK != aacEncoder_SetParam(mAACEncoder, AACENC_SAMPLERATE, mSampleRate)) {
        ALOGE("Failed to set AAC encoder parameters");
 return UNKNOWN_ERROR;
 }
 if (AACENC_OK != aacEncoder_SetParam(mAACEncoder, AACENC_BITRATE, mBitRate)) {
        ALOGE("Failed to set AAC encoder parameters");
 return UNKNOWN_ERROR;
 }
 if (AACENC_OK != aacEncoder_SetParam(mAACEncoder, AACENC_CHANNELMODE,
            getChannelMode(mNumChannels))) {
        ALOGE("Failed to set AAC encoder parameters");
 return UNKNOWN_ERROR;
 }
 if (AACENC_OK != aacEncoder_SetParam(mAACEncoder, AACENC_TRANSMUX, TT_MP4_RAW)) {
        ALOGE("Failed to set AAC encoder parameters");
 return UNKNOWN_ERROR;
 }

 if (mSBRMode != -1 && mAACProfile == OMX_AUDIO_AACObjectELD) {
 if (AACENC_OK != aacEncoder_SetParam(mAACEncoder, AACENC_SBR_MODE, mSBRMode)) {
            ALOGE("Failed to set AAC encoder parameters");
 return UNKNOWN_ERROR;
 }
 }

  
 if (AACENC_OK != aacEncoder_SetParam(mAACEncoder, AACENC_SBR_RATIO, mSBRRatio)) {
        ALOGE("Failed to set AAC encoder parameters");
 return UNKNOWN_ERROR;
 }

 return OK;
}
