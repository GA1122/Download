status_t Camera2Client::unlock() {
    ATRACE_CALL();
    ALOGV("%s: E", __FUNCTION__);
 Mutex::Autolock icl(mBinderSerializationLock);
    ALOGV("%s: Camera %d: Unlock call from pid %d; current client pid %d",
            __FUNCTION__, mCameraId, getCallingPid(), mClientPid);

 if (mClientPid == getCallingPid()) {
 SharedParameters::Lock l(mParameters);
 if (l.mParameters.state == Parameters::RECORD ||
                l.mParameters.state == Parameters::VIDEO_SNAPSHOT) {
            ALOGD("Not allowed to unlock camera during recording.");
 return INVALID_OPERATION;
 }
        mClientPid = 0;
        mRemoteCallback.clear();
        mSharedCameraCallbacks.clear();
 return OK;
 }

    ALOGE("%s: Camera %d: Unlock call from pid %d; currently locked to pid %d",
            __FUNCTION__, mCameraId, getCallingPid(), mClientPid);
 return EBUSY;
}
