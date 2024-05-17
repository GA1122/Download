SoftMP3::~SoftMP3() {
 if (mDecoderBuf != NULL) {
        free(mDecoderBuf);
        mDecoderBuf = NULL;
 }

 delete mConfig;
    mConfig = NULL;
}
