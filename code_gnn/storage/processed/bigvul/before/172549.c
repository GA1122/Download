status_t StreamingProcessor::togglePauseStream(bool pause) {
    ATRACE_CALL();
 status_t res;

    sp<CameraDeviceBase> device = mDevice.promote();
 if (device == 0) {
        ALOGE("%s: Camera %d: Device does not exist", __FUNCTION__, mId);
 return INVALID_OPERATION;
 }

    ALOGV("%s: Camera %d: toggling pause to %d", __FUNCTION__, mId, pause);

 Mutex::Autolock m(mMutex);

 if (mActiveRequest == NONE) {
        ALOGE("%s: Camera %d: Can't toggle pause, streaming was not started",
              __FUNCTION__, mId);
 return INVALID_OPERATION;
 }

 if (mPaused == pause) {
 return OK;
 }

 if (pause) {
        res = device->clearStreamingRequest();
 if (res != OK) {
            ALOGE("%s: Camera %d: Can't clear stream request: %s (%d)",
                    __FUNCTION__, mId, strerror(-res), res);
 return res;
 }
 } else {
 CameraMetadata &request =
 (mActiveRequest == PREVIEW) ? mPreviewRequest
 : mRecordingRequest;
        res = device->setStreamingRequest(request);
 if (res != OK) {
            ALOGE("%s: Camera %d: Unable to set preview request to resume: "
 "%s (%d)",
                    __FUNCTION__, mId, strerror(-res), res);
 return res;
 }
 }

    mPaused = pause;
 return OK;
}
