status_t ProCamera2Client::exclusiveTryLock() {
    ATRACE_CALL();
    ALOGV("%s", __FUNCTION__);

 Mutex::Autolock icl(mBinderSerializationLock);
 SharedCameraCallbacks::Lock l(mSharedCameraCallbacks);

 if (!mDevice.get()) return PERMISSION_DENIED;

 if (!mExclusiveLock) {
        mExclusiveLock = true;

 if (mRemoteCallback != NULL) {
            mRemoteCallback->onLockStatusChanged(
 IProCameraCallbacks::LOCK_ACQUIRED);
 }

        ALOGV("%s: exclusive lock acquired", __FUNCTION__);

 return OK;
 }


    ALOGW("%s: exclusive lock already exists - recursive locking is not"
 "allowed", __FUNCTION__);

 return ALREADY_EXISTS;
}
