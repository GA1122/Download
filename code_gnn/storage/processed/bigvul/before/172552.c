status_t StreamingProcessor::updateRecordingRequest(const Parameters &params) {
    ATRACE_CALL();
 status_t res;
 Mutex::Autolock m(mMutex);

    sp<CameraDeviceBase> device = mDevice.promote();
 if (device == 0) {
        ALOGE("%s: Camera %d: Device does not exist", __FUNCTION__, mId);
 return INVALID_OPERATION;
 }

 if (mRecordingRequest.entryCount() == 0) {
        res = device->createDefaultRequest(CAMERA2_TEMPLATE_VIDEO_RECORD,
 &mRecordingRequest);
 if (res != OK) {
            ALOGE("%s: Camera %d: Unable to create default recording request:"
 " %s (%d)", __FUNCTION__, mId, strerror(-res), res);
 return res;
 }
 }

    res = params.updateRequest(&mRecordingRequest);
 if (res != OK) {
        ALOGE("%s: Camera %d: Unable to update common entries of recording "
 "request: %s (%d)", __FUNCTION__, mId,
                strerror(-res), res);
 return res;
 }

    res = mRecordingRequest.update(ANDROID_REQUEST_ID,
 &mRecordingRequestId, 1);
 if (res != OK) {
        ALOGE("%s: Camera %d: Unable to update request id for request: %s (%d)",
                __FUNCTION__, mId, strerror(-res), res);
 return res;
 }

 return OK;
}
