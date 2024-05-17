status_t StreamingProcessor::stopStream() {
    ATRACE_CALL();
 status_t res;

 Mutex::Autolock m(mMutex);

    sp<CameraDeviceBase> device = mDevice.promote();
 if (device == 0) {
        ALOGE("%s: Camera %d: Device does not exist", __FUNCTION__, mId);
 return INVALID_OPERATION;
 }

    res = device->clearStreamingRequest();
 if (res != OK) {
        ALOGE("%s: Camera %d: Can't clear stream request: %s (%d)",
                __FUNCTION__, mId, strerror(-res), res);
 return res;
 }

    mActiveRequest = NONE;
    mActiveStreamIds.clear();
    mPaused = false;

 return OK;
}
