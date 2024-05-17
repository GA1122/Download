status_t Camera3Device::setStreamTransform(int id,
 int transform) {
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

 ssize_t idx = mOutputStreams.indexOfKey(id);
 if (idx == NAME_NOT_FOUND) {
        CLOGE("Stream %d does not exist",
                id);
 return BAD_VALUE;
 }

 return mOutputStreams.editValueAt(idx)->setTransform(transform);
}
