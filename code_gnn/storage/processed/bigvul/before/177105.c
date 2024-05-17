static int32_t BindFrameWrapper(
 void *userData, int32_t index, uint8_t **yuv) {
 SoftAVCEncoder *encoder = static_cast<SoftAVCEncoder *>(userData);
    CHECK(encoder != NULL);
 return encoder->bindOutputBuffer(index, yuv);
}
