void ProCamera2Client::onExclusiveLockStolen() {
    ALOGV("%s: ProClient lost exclusivity (id %d)",
          __FUNCTION__, mCameraId);

 Mutex::Autolock icl(mBinderSerializationLock);
 SharedCameraCallbacks::Lock l(mSharedCameraCallbacks);

 if (mExclusiveLock && mRemoteCallback.get() != NULL) {
        mRemoteCallback->onLockStatusChanged(
 IProCameraCallbacks::LOCK_STOLEN);
 }

    mExclusiveLock = false;

    detachDevice();
}
