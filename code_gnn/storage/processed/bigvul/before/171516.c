void SoftMP3::onReset() {
    pvmp3_InitDecoder(mConfig, mDecoderBuf);
    mIsFirst = true;
    mSignalledError = false;
    mSawInputEos = false;
    mSignalledOutputEos = false;
    mOutputPortSettingsChange = NONE;
}
