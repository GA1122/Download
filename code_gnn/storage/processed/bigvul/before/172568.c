bool SoftAAC2::outputDelayRingBufferPutSamples(INT_PCM *samples, int32_t numSamples) {
 if (numSamples == 0) {
 return true;
 }
 if (outputDelayRingBufferSpaceLeft() < numSamples) {
        ALOGE("RING BUFFER WOULD OVERFLOW");
 return false;
 }
 if (mOutputDelayRingBufferWritePos + numSamples <= mOutputDelayRingBufferSize
 && (mOutputDelayRingBufferReadPos <= mOutputDelayRingBufferWritePos
 || mOutputDelayRingBufferReadPos > mOutputDelayRingBufferWritePos + numSamples)) {
 for (int32_t i = 0; i < numSamples; i++) {
            mOutputDelayRingBuffer[mOutputDelayRingBufferWritePos++] = samples[i];
 }

 if (mOutputDelayRingBufferWritePos >= mOutputDelayRingBufferSize) {
            mOutputDelayRingBufferWritePos -= mOutputDelayRingBufferSize;
 }
 } else {
        ALOGV("slow SoftAAC2::outputDelayRingBufferPutSamples()");

 for (int32_t i = 0; i < numSamples; i++) {
            mOutputDelayRingBuffer[mOutputDelayRingBufferWritePos] = samples[i];
            mOutputDelayRingBufferWritePos++;
 if (mOutputDelayRingBufferWritePos >= mOutputDelayRingBufferSize) {
                mOutputDelayRingBufferWritePos -= mOutputDelayRingBufferSize;
 }
 }
 }
    mOutputDelayRingBufferFilled += numSamples;
 return true;
}
