void SoftAACEncoder2::onReset() {
 delete[] mInputFrame;
    mInputFrame = NULL;
    mInputSize = 0;

    mSentCodecSpecificData = false;
    mInputTimeUs = -1ll;
    mSawInputEOS = false;
    mSignalledError = false;
}
