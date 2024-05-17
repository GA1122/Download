int32_t SoftAVCEncoder::allocOutputBuffers(
 unsigned int sizeInMbs, unsigned int numBuffers) {
    CHECK(mOutputBuffers.isEmpty());
 size_t frameSize = (sizeInMbs << 7) * 3;
 for (unsigned int i = 0; i <  numBuffers; ++i) {
 MediaBuffer *buffer = new MediaBuffer(frameSize);
        buffer->setObserver(this);
        mOutputBuffers.push(buffer);
 }

 return 1;
}
