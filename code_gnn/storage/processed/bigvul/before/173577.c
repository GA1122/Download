void NuPlayer::GenericSource::finishPrepareAsync() {
 status_t err = startSources();
 if (err != OK) {
        ALOGE("Failed to init start data source!");
        notifyPreparedAndCleanup(err);
 return;
 }

 if (mIsStreaming) {
        mPrepareBuffering = true;

        ensureCacheIsFetching();
        restartPollBuffering();
 } else {
        notifyPrepared();
 }
}
