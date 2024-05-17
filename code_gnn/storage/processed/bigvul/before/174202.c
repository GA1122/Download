status_t Camera3Device::clearStreamingRequest(int64_t *lastFrameNumber) {
    ATRACE_CALL();
 Mutex::Autolock il(mInterfaceLock);
 Mutex::Autolock l(mLock);

 switch (mStatus) {
 case STATUS_ERROR:
            CLOGE("Device has encountered a serious error");
 return INVALID_OPERATION;
 case STATUS_UNINITIALIZED:
            CLOGE("Device not initialized");
 return INVALID_OPERATION;
 case STATUS_UNCONFIGURED:
 case STATUS_CONFIGURED:
 case STATUS_ACTIVE:
 break;
 default:
            SET_ERR_L("Unexpected status: %d", mStatus);
 return INVALID_OPERATION;
 }
    ALOGV("Camera %d: Clearing repeating request", mId);

 return mRequestThread->clearRepeatingRequests(lastFrameNumber);
}
