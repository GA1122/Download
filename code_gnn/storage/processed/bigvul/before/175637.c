status_t SoftAACEncoder::setAudioParams() {

    ALOGV("setAudioParams: %lu Hz, %lu channels, %lu bps",
         mSampleRate, mNumChannels, mBitRate);

 status_t err = setAudioSpecificConfigData();

 if (err != OK) {
 return err;
 }

    AACENC_PARAM params;
    memset(&params, 0, sizeof(params));
    params.sampleRate = mSampleRate;
    params.bitRate = mBitRate;
    params.nChannels = mNumChannels;
    params.adtsUsed = 0;  
 if (VO_ERR_NONE != mApiHandle->SetParam(
                mEncoderHandle, VO_PID_AAC_ENCPARAM, &params)) {
        ALOGE("Failed to set AAC encoder parameters");
 return UNKNOWN_ERROR;
 }

 return OK;
}
