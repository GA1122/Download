status_t AudioSource::dataCallback(const AudioRecord::Buffer& audioBuffer) {
 int64_t timeUs = systemTime() / 1000ll;
    timeUs -= mRecord->latency() * 1000LL;

    ALOGV("dataCallbackTimestamp: %" PRId64 " us", timeUs);
 Mutex::Autolock autoLock(mLock);
 if (!mStarted) {
        ALOGW("Spurious callback from AudioRecord. Drop the audio data.");
 return OK;
 }

 if (mNumFramesReceived == 0 && timeUs < mStartTimeUs) {
 (void) mRecord->getInputFramesLost();
        ALOGV("Drop audio data at %" PRId64 "/%" PRId64 " us", timeUs, mStartTimeUs);
 return OK;
 }

 if (mNumFramesReceived == 0 && mPrevSampleTimeUs == 0) {
        mInitialReadTimeUs = timeUs;
 if (mStartTimeUs > 0) {
            mStartTimeUs = timeUs - mStartTimeUs;
 } else {
            mStartTimeUs += mRecord->latency() * 1000;
 }

        mPrevSampleTimeUs = mStartTimeUs;
 }

 size_t numLostBytes = 0;
 if (mNumFramesReceived > 0) {  
        numLostBytes = mRecord->getInputFramesLost() * mRecord->frameSize();
 }

    CHECK_EQ(numLostBytes & 1, 0u);
    CHECK_EQ(audioBuffer.size & 1, 0u);
 if (numLostBytes > 0) {
        ALOGW("Lost audio record data: %zu bytes", numLostBytes);
 }

 while (numLostBytes > 0) {
 size_t bufferSize = numLostBytes;
 if (numLostBytes > kMaxBufferSize) {
            numLostBytes -= kMaxBufferSize;
            bufferSize = kMaxBufferSize;
 } else {
            numLostBytes = 0;
 }
 MediaBuffer *lostAudioBuffer = new MediaBuffer(bufferSize);
        memset(lostAudioBuffer->data(), 0, bufferSize);
        lostAudioBuffer->set_range(0, bufferSize);
        queueInputBuffer_l(lostAudioBuffer, timeUs);
 }

 if (audioBuffer.size == 0) {
        ALOGW("Nothing is available from AudioRecord callback buffer");
 return OK;
 }

 const size_t bufferSize = audioBuffer.size;
 MediaBuffer *buffer = new MediaBuffer(bufferSize);
    memcpy((uint8_t *) buffer->data(),
            audioBuffer.i16, audioBuffer.size);
    buffer->set_range(0, bufferSize);
    queueInputBuffer_l(buffer, timeUs);
 return OK;
}
