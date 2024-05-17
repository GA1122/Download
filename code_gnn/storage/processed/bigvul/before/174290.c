bool Camera3Device::RequestThread::waitIfPaused() {
 status_t res;
 Mutex::Autolock l(mPauseLock);
 while (mDoPause) {
 if (mPaused == false) {
            mPaused = true;
            ALOGV("%s: RequestThread: Paused", __FUNCTION__);
            sp<StatusTracker> statusTracker = mStatusTracker.promote();
 if (statusTracker != 0) {
                statusTracker->markComponentIdle(mStatusId, Fence::NO_FENCE);
 }
 }

        res = mDoPauseSignal.waitRelative(mPauseLock, kRequestTimeout);
 if (res == TIMED_OUT || exitPending()) {
 return true;
 }
 }
 return false;
}
