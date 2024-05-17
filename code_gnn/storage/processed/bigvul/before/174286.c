void Camera3Device::RequestThread::unpauseForNewRequests() {
    mRequestSignal.signal();
 Mutex::Autolock p(mPauseLock);
 if (!mDoPause) {
        ALOGV("%s: RequestThread: Going active", __FUNCTION__);
 if (mPaused) {
            sp<StatusTracker> statusTracker = mStatusTracker.promote();
 if (statusTracker != 0) {
                statusTracker->markComponentActive(mStatusId);
 }
 }
        mPaused = false;
 }
}
