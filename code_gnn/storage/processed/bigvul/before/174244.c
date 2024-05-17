void Camera3Device::notifyStatus(bool idle) {
 {
 Mutex::Autolock l(mLock);
 if (mStatus != STATUS_ACTIVE && mStatus != STATUS_CONFIGURED) {
 return;
 }
        ALOGV("%s: Camera %d: Now %s", __FUNCTION__, mId,
                idle ? "idle" : "active");
        internalUpdateStatusLocked(idle ? STATUS_CONFIGURED : STATUS_ACTIVE);

 if (mPauseStateNotify) return;
 }
 NotificationListener *listener;
 {
 Mutex::Autolock l(mOutputLock);
        listener = mListener;
 }
 if (idle && listener != NULL) {
        listener->notifyIdle();
 }
}
