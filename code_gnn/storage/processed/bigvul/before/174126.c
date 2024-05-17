SoftAMR::~SoftAMR() {
 if (mMode == MODE_NARROW) {
 GSMDecodeFrameExit(&mState);
        mState = NULL;
 } else {
        free(mDecoderBuf);
        mDecoderBuf = NULL;

        mState = NULL;
        mDecoderCookie = NULL;
 }
}
