status_t Camera3Device::createZslStream(
 uint32_t width, uint32_t height,
 int depth,
  
 int *id,
            sp<Camera3ZslStream>* zslStream) {
    ATRACE_CALL();
 Mutex::Autolock il(mInterfaceLock);
 Mutex::Autolock l(mLock);
    ALOGV("Camera %d: Creating ZSL stream %d: %d x %d, depth %d",
            mId, mNextStreamId, width, height, depth);

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
            SET_ERR_L("Unexpected status: %d", mStatus);
 return INVALID_OPERATION;
 }
    assert(mStatus != STATUS_ACTIVE);

 if (mInputStream != 0) {
        ALOGE("%s: Cannot create more than 1 input stream", __FUNCTION__);
 return INVALID_OPERATION;
 }

    sp<Camera3ZslStream> newStream = new Camera3ZslStream(mNextStreamId,
                width, height, depth);
    newStream->setStatusTracker(mStatusTracker);

    res = mOutputStreams.add(mNextStreamId, newStream);
 if (res < 0) {
        ALOGE("%s: Can't add new stream to set: %s (%d)",
                __FUNCTION__, strerror(-res), res);
 return res;
 }
    mInputStream = newStream;

    mNeedConfig = true;

 *id = mNextStreamId++;
 *zslStream = newStream;

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

    ALOGV("Camera %d: Created ZSL stream", mId);
 return OK;
}
