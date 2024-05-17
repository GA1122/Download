status_t Camera3Device::flush(int64_t *frameNumber) {
    ATRACE_CALL();
    ALOGV("%s: Camera %d: Flushing all requests", __FUNCTION__, mId);
 Mutex::Autolock il(mInterfaceLock);

 NotificationListener* listener;
 {
 Mutex::Autolock l(mOutputLock);
        listener = mListener;
 }

 {
 Mutex::Autolock l(mLock);
        mRequestThread->clear(listener,  frameNumber);
 }

 status_t res;
 if (mHal3Device->common.version >= CAMERA_DEVICE_API_VERSION_3_1) {
        res = mRequestThread->flush();
 } else {
 Mutex::Autolock l(mLock);
        res = waitUntilDrainedLocked();
 }

 return res;
}
