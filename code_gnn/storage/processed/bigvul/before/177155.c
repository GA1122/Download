SoftFlacEncoder::~SoftFlacEncoder() {
    ALOGV("SoftFlacEncoder::~SoftFlacEncoder()");
 if (mFlacStreamEncoder != NULL) {
        FLAC__stream_encoder_delete(mFlacStreamEncoder);
        mFlacStreamEncoder = NULL;
 }
    free(mInputBufferPcm32);
    mInputBufferPcm32 = NULL;
}
