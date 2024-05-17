void NuPlayer::GenericSource::startBufferingIfNecessary() {
    ALOGV("startBufferingIfNecessary: mPrepareBuffering=%d, mBuffering=%d",
            mPrepareBuffering, mBuffering);

 if (mPrepareBuffering) {
 return;
 }

 if (!mBuffering) {
        mBuffering = true;

        ensureCacheIsFetching();
        sendCacheStats();

        sp<AMessage> notify = dupNotify();
        notify->setInt32("what", kWhatPauseOnBufferingStart);
        notify->post();
 }
}
