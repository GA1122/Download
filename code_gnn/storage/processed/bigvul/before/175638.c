status_t SoftAACEncoder::setAudioSpecificConfigData() {

 int32_t index;
 status_t err = getSampleRateTableIndex(mSampleRate, index);
 if (err != OK) {
        ALOGE("Unsupported sample rate (%lu Hz)", mSampleRate);
 return err;
 }

 if (mNumChannels > 2 || mNumChannels <= 0) {
        ALOGE("Unsupported number of channels(%lu)", mNumChannels);
 return UNKNOWN_ERROR;
 }

    mAudioSpecificConfigData[0] = ((0x02 << 3) | (index >> 1));
    mAudioSpecificConfigData[1] = ((index & 0x01) << 7) | (mNumChannels << 3);

 return OK;
}
