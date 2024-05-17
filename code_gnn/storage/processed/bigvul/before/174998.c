status_t ProCamera2Client::cancelRequest(int requestId) {
 (void)requestId;
    ATRACE_CALL();
    ALOGV("%s", __FUNCTION__);

 Mutex::Autolock icl(mBinderSerializationLock);

 if (!mDevice.get()) return DEAD_OBJECT;

 if (!mExclusiveLock) {
 return PERMISSION_DENIED;
 }

    ALOGE("%s: not fully implemented yet", __FUNCTION__);
 return INVALID_OPERATION;
}
