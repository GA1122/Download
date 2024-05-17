sp<Camera3Device::CaptureRequest> Camera3Device::setUpRequestLocked(
 const CameraMetadata &request) {
 status_t res;

 if (mStatus == STATUS_UNCONFIGURED || mNeedConfig) {
        res = configureStreamsLocked();
 if (res == BAD_VALUE && mStatus == STATUS_UNCONFIGURED) {
            CLOGE("No streams configured");
 return NULL;
 }
 if (res != OK) {
            SET_ERR_L("Can't set up streams: %s (%d)", strerror(-res), res);
 return NULL;
 }
 if (mStatus == STATUS_UNCONFIGURED) {
            CLOGE("No streams configured");
 return NULL;
 }
 }

    sp<CaptureRequest> newRequest = createCaptureRequest(request);
 return newRequest;
}
