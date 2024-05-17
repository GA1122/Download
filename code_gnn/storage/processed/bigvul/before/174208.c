status_t Camera3Device::createInputStream(
 uint32_t width, uint32_t height, int format, int *id) {
    ATRACE_CALL();
 Mutex::Autolock il(mInterfaceLock);
 Mutex::Autolock l(mLock);
    ALOGV("Camera %d: Creating new input stream %d: %d x %d, format %d",
            mId, mNextStreamId, width, height, format);

 status_t res;
 bool wasActive = false;

 switch (mStatus) {
 case STATUS_ERROR:
            ALOGE("%s: Device has encountered a serious error", __FUNCTION__);
 return INVALID_OPERATION;
 case STATUS_UNINITIALIZED:
            ALOGE("%s: Device not initialized", __FUNCTION__);
 return INVALID_OPERATION;
 case STATUS_UNCONFIGURED:
 case STATUS_CONFIGURED:
 break;
 case STATUS_ACTIVE:
            ALOGV("%s: Stopping activity to reconfigure streams", __FUNCTION__);
            res = internalPauseAndWaitLocked();
 if (res != OK) {
                SET_ERR_L("Can't pause captures to reconfigure streams!");
 return res;
 }
            wasActive = true;
 break;
 default:
            SET_ERR_L("%s: Unexpected status: %d", mStatus);
 return INVALID_OPERATION;
 }
    assert(mStatus != STATUS_ACTIVE);

 if (mInputStream != 0) {
        ALOGE("%s: Cannot create more than 1 input stream", __FUNCTION__);
 return INVALID_OPERATION;
 }

    sp<Camera3InputStream> newStream = new Camera3InputStream(mNextStreamId,
                width, height, format);
    newStream->setStatusTracker(mStatusTracker);

    mInputStream = newStream;

 *id = mNextStreamId++;

 if (wasActive) {
        ALOGV("%s: Restarting activity to reconfigure streams", __FUNCTION__);
        res = configureStreamsLocked();
 if (res != OK) {
            ALOGE("%s: Can't reconfigure device for new stream %d: %s (%d)",
                    __FUNCTION__, mNextStreamId, strerror(-res), res);
 return res;
 }
        internalResumeLocked();
 }

    ALOGV("Camera %d: Created input stream", mId);
 return OK;
}
