void MediaPlayerService::AudioOutput::switchToNextOutput() {
    ALOGV("switchToNextOutput");

 const unsigned kMaxSwitchTries = 100;
 Mutex::Autolock lock(mLock);
 for (unsigned tries = 0;;) {
 if (mTrack == 0) {
 return;
 }
 if (mNextOutput != NULL && mNextOutput != this) {
 if (mCallbackData != NULL) {
#if 1
 CallbackData *callbackData = mCallbackData;
                mLock.unlock();
                callbackData->lock();
                mLock.lock();
 if (callbackData != mCallbackData || mNextOutput == NULL || mNextOutput == this) {
                    LOG_ALWAYS_FATAL_IF(++tries > kMaxSwitchTries,
 "switchToNextOutput() cannot obtain correct lock sequence");
                    callbackData->unlock();
 continue;
 }
                callbackData->mSwitching = true;  
#else
 if (!mCallbackData->tryBeginTrackSwitch()) {
                    LOG_ALWAYS_FATAL_IF(++tries > kMaxSwitchTries,
 "switchToNextOutput() cannot obtain callback lock");
                    mLock.unlock();
                    usleep(5 * 1000  );  
                    mLock.lock();
 continue;
 }
#endif
 }

 Mutex::Autolock nextLock(mNextOutput->mLock);

 if (mNextOutput->mTrack == NULL) {
                ALOGD("Recycling track for gapless playback");
 delete mNextOutput->mCallbackData;
                mNextOutput->mCallbackData = mCallbackData;
                mNextOutput->mRecycledTrack = mTrack;
                mNextOutput->mSampleRateHz = mSampleRateHz;
                mNextOutput->mMsecsPerFrame = mMsecsPerFrame;
                mNextOutput->mFlags = mFlags;
                mNextOutput->mFrameSize = mFrameSize;
                close_l();
                mCallbackData = NULL;  
 } else {
                ALOGW("Ignoring gapless playback because next player has already started");
 if (mCallbackData != NULL) {
                    mCallbackData->endTrackSwitch();  
 }
                close_l();
 }
 }
 break;
 }
}
