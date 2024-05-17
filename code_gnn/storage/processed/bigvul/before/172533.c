status_t StreamingProcessor::deleteRecordingStream() {
    ATRACE_CALL();
 status_t res;

 Mutex::Autolock m(mMutex);

 if (mRecordingStreamId != NO_STREAM) {
        sp<CameraDeviceBase> device = mDevice.promote();
 if (device == 0) {
            ALOGE("%s: Camera %d: Device does not exist", __FUNCTION__, mId);
 return INVALID_OPERATION;
 }

        res = device->waitUntilDrained();
 if (res != OK) {
            ALOGE("%s: Error waiting for HAL to drain: %s (%d)",
                    __FUNCTION__, strerror(-res), res);
 return res;
 }
        res = device->deleteStream(mRecordingStreamId);
 if (res != OK) {
            ALOGE("%s: Unable to delete recording stream: %s (%d)",
                    __FUNCTION__, strerror(-res), res);
 return res;
 }
        mRecordingStreamId = NO_STREAM;
 }
 return OK;
}
