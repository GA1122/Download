void SoftVorbis::onReset() {
    mInputBufferCount = 0;
    mNumFramesOutput = 0;
 if (mState != NULL) {
        vorbis_dsp_clear(mState);
 delete mState;
        mState = NULL;
 }

 if (mVi != NULL) {
        vorbis_info_clear(mVi);
 delete mVi;
        mVi = NULL;
 }

    mSawInputEos = false;
    mSignalledOutputEos = false;
    mOutputPortSettingsChange = NONE;
}
