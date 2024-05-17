status_t Camera3Device::createStream(sp<Surface> consumer,
 uint32_t width, uint32_t height, int format, android_dataspace dataSpace,
 camera3_stream_rotation_t rotation, int *id) {
    ATRACE_CALL();
 Mutex::Autolock il(mInterfaceLock);
 Mutex::Autolock l(mLock);
    ALOGV("Camera %d: Creating new stream %d: %d x %d, format %d, dataspace %d rotation %d",
            mId, mNextStreamId, width, height, format, dataSpace, rotation);

 status_t res;
 bool wasActive = false;

 switch (mStatus) {
 case STATUS_ERROR:
            CLOGE("Device has encountered a serious error");
 return INVALID_OPERATION;
 case STATUS_UNINITIALIZED:
            CLOGE("Device not initialized");
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

    sp<Camera3OutputStream> newStream;
 if (format == HAL_PIXEL_FORMAT_BLOB) {
 ssize_t blobBufferSize;
 if (dataSpace != HAL_DATASPACE_DEPTH) {
            blobBufferSize = getJpegBufferSize(width, height);
 if (blobBufferSize <= 0) {
                SET_ERR_L("Invalid jpeg buffer size %zd", blobBufferSize);
 return BAD_VALUE;
 }
 } else {
            blobBufferSize = getPointCloudBufferSize();
 if (blobBufferSize <= 0) {
                SET_ERR_L("Invalid point cloud buffer size %zd", blobBufferSize);
 return BAD_VALUE;
 }
 }
        newStream = new Camera3OutputStream(mNextStreamId, consumer,
                width, height, blobBufferSize, format, dataSpace, rotation);
 } else {
        newStream = new Camera3OutputStream(mNextStreamId, consumer,
                width, height, format, dataSpace, rotation);
 }
    newStream->setStatusTracker(mStatusTracker);

    res = mOutputStreams.add(mNextStreamId, newStream);
 if (res < 0) {
        SET_ERR_L("Can't add new stream to set: %s (%d)", strerror(-res), res);
 return res;
 }

 *id = mNextStreamId++;
    mNeedConfig = true;

 if (wasActive) {
        ALOGV("%s: Restarting activity to reconfigure streams", __FUNCTION__);
        res = configureStreamsLocked();
 if (res != OK) {
            CLOGE("Can't reconfigure device for new stream %d: %s (%d)",
                    mNextStreamId, strerror(-res), res);
 return res;
 }
        internalResumeLocked();
 }
    ALOGV("Camera %d: Created new stream", mId);
 return OK;
}
