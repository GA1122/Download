void SoftAAC2::onReset() {
    drainDecoder();
    mInputBufferCount = 0;
    mOutputBufferCount = 0;
    mOutputDelayCompensated = 0;
    mOutputDelayRingBufferWritePos = 0;
    mOutputDelayRingBufferReadPos = 0;
    mOutputDelayRingBufferFilled = 0;
    mEndOfInput = false;
    mEndOfOutput = false;
    mBufferTimestamps.clear();
    mBufferSizes.clear();
    mDecodedSizes.clear();
    mLastInHeader = NULL;

    mStreamInfo->sampleRate = 0;  

    mSignalledError = false;
    mOutputPortSettingsChange = NONE;
}
