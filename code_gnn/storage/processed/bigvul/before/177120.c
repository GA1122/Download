void SoftAVCEncoder::releaseOutputBuffers() {
 for (size_t i = 0; i < mOutputBuffers.size(); ++i) {
 MediaBuffer *buffer = mOutputBuffers.editItemAt(i);
        buffer->setObserver(NULL);
        buffer->release();
 }
    mOutputBuffers.clear();
}
