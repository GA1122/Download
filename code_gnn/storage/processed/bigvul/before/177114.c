int32_t SoftAVCEncoder::bindOutputBuffer(int32_t index, uint8_t **yuv) {
    CHECK(index >= 0);
    CHECK(index < (int32_t) mOutputBuffers.size());
 *yuv = (uint8_t *) mOutputBuffers[index]->data();

 return 1;
}
