void SoftAAC2::onPortFlushCompleted(OMX_U32 portIndex) {
 if (portIndex == 0) {
        drainDecoder();
        mBufferTimestamps.clear();
        mBufferSizes.clear();
        mDecodedSizes.clear();
        mLastInHeader = NULL;
        mEndOfInput = false;
 } else {
 int avail;
 while ((avail = outputDelayRingBufferSamplesAvailable()) > 0) {
 if (avail > mStreamInfo->frameSize * mStreamInfo->numChannels) {
                avail = mStreamInfo->frameSize * mStreamInfo->numChannels;
 }
 int32_t ns = outputDelayRingBufferGetSamples(0, avail);
 if (ns != avail) {
                ALOGW("not a complete frame of samples available");
 break;
 }
            mOutputBufferCount++;
 }
        mOutputDelayRingBufferReadPos = mOutputDelayRingBufferWritePos;
        mEndOfOutput = false;
 }
}
