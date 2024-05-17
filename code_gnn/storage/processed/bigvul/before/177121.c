void SoftAVCEncoder::signalBufferReturned(MediaBuffer *buffer) {
    UNUSED_UNLESS_VERBOSE(buffer);
    ALOGV("signalBufferReturned: %p", buffer);
}
