void NuPlayer::GenericSource::onPollBuffering() {
 status_t finalStatus = UNKNOWN_ERROR;
 int64_t cachedDurationUs = 0ll;

 if (mCachedSource != NULL) {
 size_t cachedDataRemaining =
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
 } else if (mWVMExtractor != NULL) {
        cachedDurationUs
 = mWVMExtractor->getCachedDurationUs(&finalStatus);
 }

 if (finalStatus == ERROR_END_OF_STREAM) {
        notifyBufferingUpdate(100);
        cancelPollBuffering();
 return;
 } else if (cachedDurationUs > 0ll && mDurationUs > 0ll) {
 int percentage = 100.0 * cachedDurationUs / mDurationUs;
 if (percentage > 100) {
            percentage = 100;
 }

        notifyBufferingUpdate(percentage);
 }

    schedulePollBuffering();
}
