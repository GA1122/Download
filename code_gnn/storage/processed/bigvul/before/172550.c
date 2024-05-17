status_t StreamingProcessor::updatePreviewRequest(const Parameters &params) {
    ATRACE_CALL();
 status_t res;
    sp<CameraDeviceBase> device = mDevice.promote();
 if (device == 0) {
        ALOGE("%s: Camera %d: Device does not exist", __FUNCTION__, mId);
 return INVALID_OPERATION;
 }

 Mutex::Autolock m(mMutex);
 if (mPreviewRequest.entryCount() == 0) {
        sp<Camera2Client> client = mClient.promote();
 if (client == 0) {
            ALOGE("%s: Camera %d: Client does not exist", __FUNCTION__, mId);
 return INVALID_OPERATION;
 }

 if (client->getCameraDeviceVersion() >= CAMERA_DEVICE_API_VERSION_3_0) {
 if (params.zslMode && !params.recordingHint) {
                res = device->createDefaultRequest(CAMERA3_TEMPLATE_ZERO_SHUTTER_LAG,
 &mPreviewRequest);
 } else {
                res = device->createDefaultRequest(CAMERA3_TEMPLATE_PREVIEW,
 &mPreviewRequest);
 }
 } else {
            res = device->createDefaultRequest(CAMERA2_TEMPLATE_PREVIEW,
 &mPreviewRequest);
 }

 if (res != OK) {
            ALOGE("%s: Camera %d: Unable to create default preview request: "
 "%s (%d)", __FUNCTION__, mId, strerror(-res), res);
 return res;
 }
 }

    res = params.updateRequest(&mPreviewRequest);
 if (res != OK) {
        ALOGE("%s: Camera %d: Unable to update common entries of preview "
 "request: %s (%d)", __FUNCTION__, mId,
                strerror(-res), res);
 return res;
 }

    res = mPreviewRequest.update(ANDROID_REQUEST_ID,
 &mPreviewRequestId, 1);
 if (res != OK) {
        ALOGE("%s: Camera %d: Unable to update request id for preview: %s (%d)",
                __FUNCTION__, mId, strerror(-res), res);
 return res;
 }

 return OK;
}
