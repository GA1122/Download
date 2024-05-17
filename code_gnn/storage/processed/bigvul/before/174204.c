status_t Camera3Device::configureStreams(bool isConstrainedHighSpeed) {
    ATRACE_CALL();
    ALOGV("%s: E", __FUNCTION__);

 Mutex::Autolock il(mInterfaceLock);
 Mutex::Autolock l(mLock);

 if (mIsConstrainedHighSpeedConfiguration != isConstrainedHighSpeed) {
        mNeedConfig = true;
        mIsConstrainedHighSpeedConfiguration = isConstrainedHighSpeed;
 }

 return configureStreamsLocked();
}
