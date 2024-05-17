int32_t SoftAAC2::outputDelayRingBufferSpaceLeft() {
 return mOutputDelayRingBufferSize - outputDelayRingBufferSamplesAvailable();
}
