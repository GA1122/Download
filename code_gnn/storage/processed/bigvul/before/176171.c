uint8_t* AMediaCodec_getOutputBuffer(AMediaCodec *mData, size_t idx, size_t *out_size) {
 if (mData->mAsyncNotify != NULL) {
        sp<MediaCodecBuffer> abuf;
 if (mData->mCodec->getOutputBuffer(idx, &abuf) != 0) {
 return NULL;
 }

 if (out_size != NULL) {
 *out_size = abuf->capacity();
 }
 return abuf->data();
 }

    android::Vector<android::sp<android::MediaCodecBuffer> > abufs;
 if (mData->mCodec->getOutputBuffers(&abufs) == 0) {
 size_t n = abufs.size();
 if (idx >= n) {
            ALOGE("buffer index %zu out of range", idx);
 return NULL;
 }
 if (out_size != NULL) {
 *out_size = abufs[idx]->capacity();
 }
 return abufs[idx]->data();
 }
    ALOGE("couldn't get output buffers");
 return NULL;
}
