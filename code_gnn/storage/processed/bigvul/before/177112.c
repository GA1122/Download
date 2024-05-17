static void UnbindFrameWrapper(void *userData, int32_t index) {
 SoftAVCEncoder *encoder = static_cast<SoftAVCEncoder *>(userData);
    CHECK(encoder != NULL);
 return encoder->unbindOutputBuffer(index);
}
