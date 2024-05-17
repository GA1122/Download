void NuPlayer::GenericSource::onPollBuffering() {
 status_t finalStatus = UNKNOWN_ERROR;
 int64_t cachedDurationUs = -1ll;
 ssize_t cachedDataRemaining = -1;

    ALOGW_IF(mWVMExtractor != NULL && mCachedSource != NULL,
 "WVMExtractor and NuCachedSource both present");

 if (mWVMExtractor != NULL) {
        cachedDurationUs =
                mWVMExtractor->getCachedDurationUs(&finalStatus);
 } else if (mCachedSource != NULL) {
        cachedDataRemaining =
                mCachedSource->approxDataRemaining(&finalStatus);

 if (finalStatus == OK) {
 off64_t size;
 int64_t bitrate = 0ll;
 if (mDurationUs > 0 && mCachedSource->getSize(&size) == OK) {
                bitrate = size * 8000000ll / mDurationUs;
 } else if (mBitrate > 0) {
                bitrate = mBitrate;
 }
 if (bitrate > 0) {
                cachedDurationUs = cachedDataRemaining * 8000000ll / bitrate;
 }
 }
 }

 if (finalStatus != OK) {
        ALOGV("onPollBuffering: EOS (finalStatus = %d)", finalStatus);

 if (finalStatus == ERROR_END_OF_STREAM) {
            notifyBufferingUpdate(100);
 }

        stopBufferingIfNecessary();
 return;
 } else if (cachedDurationUs >= 0ll) {
 if (mDurationUs > 0ll) {
 int64_t cachedPosUs = getLastReadPosition() + cachedDurationUs;
 int percentage = 100.0 * cachedPosUs / mDurationUs;
 if (percentage > 100) {
                percentage = 100;
 }

            notifyBufferingUpdate(percentage);
 }

        ALOGV("onPollBuffering: cachedDurationUs %.1f sec",
                cachedDurationUs / 1000000.0f);

 if (cachedDurationUs < kLowWaterMarkUs) {
            startBufferingIfNecessary();
 } else if (cachedDurationUs > kHighWaterMarkUs) {
            stopBufferingIfNecessary();
 }
 } else if (cachedDataRemaining >= 0) {
        ALOGV("onPollBuffering: cachedDataRemaining %zd bytes",
                cachedDataRemaining);

 if (cachedDataRemaining < kLowWaterMarkBytes) {
            startBufferingIfNecessary();
 } else if (cachedDataRemaining > kHighWaterMarkBytes) {
            stopBufferingIfNecessary();
 }
 }

    schedulePollBuffering();
}
