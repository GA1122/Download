status_t StreamingProcessor::updatePreviewStream(const Parameters &params) {
    ATRACE_CALL();
 Mutex::Autolock m(mMutex);

 status_t res;
    sp<CameraDeviceBase> device = mDevice.promote();
 if (device == 0) {
        ALOGE("%s: Camera %d: Device does not exist", __FUNCTION__, mId);
 return INVALID_OPERATION;
 }

 if (mPreviewStreamId != NO_STREAM) {
 uint32_t currentWidth, currentHeight;
        res = device->getStreamInfo(mPreviewStreamId,
 &currentWidth, &currentHeight, 0, 0);
 if (res != OK) {
            ALOGE("%s: Camera %d: Error querying preview stream info: "
 "%s (%d)", __FUNCTION__, mId, strerror(-res), res);
 return res;
 }
 if (currentWidth != (uint32_t)params.previewWidth ||
                currentHeight != (uint32_t)params.previewHeight) {
            ALOGV("%s: Camera %d: Preview size switch: %d x %d -> %d x %d",
                    __FUNCTION__, mId, currentWidth, currentHeight,
                    params.previewWidth, params.previewHeight);
            res = device->waitUntilDrained();
 if (res != OK) {
                ALOGE("%s: Camera %d: Error waiting for preview to drain: "
 "%s (%d)", __FUNCTION__, mId, strerror(-res), res);
 return res;
 }
            res = device->deleteStream(mPreviewStreamId);
 if (res != OK) {
                ALOGE("%s: Camera %d: Unable to delete old output stream "
 "for preview: %s (%d)", __FUNCTION__, mId,
                        strerror(-res), res);
 return res;
 }
            mPreviewStreamId = NO_STREAM;
 }
 }

 if (mPreviewStreamId == NO_STREAM) {
        res = device->createStream(mPreviewWindow,
                params.previewWidth, params.previewHeight,
                CAMERA2_HAL_PIXEL_FORMAT_OPAQUE, HAL_DATASPACE_UNKNOWN,
                CAMERA3_STREAM_ROTATION_0, &mPreviewStreamId);
 if (res != OK) {
            ALOGE("%s: Camera %d: Unable to create preview stream: %s (%d)",
                    __FUNCTION__, mId, strerror(-res), res);
 return res;
 }
 }

    res = device->setStreamTransform(mPreviewStreamId,
            params.previewTransform);
 if (res != OK) {
        ALOGE("%s: Camera %d: Unable to set preview stream transform: "
 "%s (%d)", __FUNCTION__, mId, strerror(-res), res);
 return res;
 }

 return OK;
}
