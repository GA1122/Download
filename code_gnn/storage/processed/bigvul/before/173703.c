status_t AudioSource::read(
 MediaBuffer **out, const ReadOptions *  ) {
 Mutex::Autolock autoLock(mLock);
 *out = NULL;

 if (mInitCheck != OK) {
 return NO_INIT;
 }

 while (mStarted && mBuffersReceived.empty()) {
        mFrameAvailableCondition.wait(mLock);
 }
 if (!mStarted) {
 return OK;
 }
 MediaBuffer *buffer = *mBuffersReceived.begin();
    mBuffersReceived.erase(mBuffersReceived.begin());
 ++mNumClientOwnedBuffers;
    buffer->setObserver(this);
    buffer->add_ref();

 int64_t timeUs;
    CHECK(buffer->meta_data()->findInt64(kKeyTime, &timeUs));
 int64_t elapsedTimeUs = timeUs - mStartTimeUs;
 if (elapsedTimeUs < kAutoRampStartUs) {
        memset((uint8_t *) buffer->data(), 0, buffer->range_length());
 } else if (elapsedTimeUs < kAutoRampStartUs + kAutoRampDurationUs) {
 int32_t autoRampDurationFrames =
 ((int64_t)kAutoRampDurationUs * mSampleRate + 500000LL) / 1000000LL;  

 int32_t autoRampStartFrames =
 ((int64_t)kAutoRampStartUs * mSampleRate + 500000LL) / 1000000LL;  

 int32_t nFrames = mNumFramesReceived - autoRampStartFrames;
        rampVolume(nFrames, autoRampDurationFrames,
 (uint8_t *) buffer->data(), buffer->range_length());
 }

 if (mTrackMaxAmplitude) {
        trackMaxAmplitude(
 (int16_t *) buffer->data(), buffer->range_length() >> 1);
 }

 if (mSampleRate != mOutSampleRate) {
 if (mFirstSampleTimeUs < 0) {
            mFirstSampleTimeUs = timeUs;
 }
        timeUs = mFirstSampleTimeUs + (timeUs - mFirstSampleTimeUs)
 * (int64_t)mSampleRate / (int64_t)mOutSampleRate;
        buffer->meta_data()->setInt64(kKeyTime, timeUs);
 }

 *out = buffer;
 return OK;
}
