status_t CameraSource::configureCamera(
 CameraParameters* params,
 int32_t width, int32_t height,
 int32_t frameRate) {
    ALOGV("configureCamera");
 Vector<Size> sizes;
 bool isSetVideoSizeSupportedByCamera = true;
    getSupportedVideoSizes(*params, &isSetVideoSizeSupportedByCamera, sizes);
 bool isCameraParamChanged = false;
 if (width != -1 && height != -1) {
 if (!isVideoSizeSupported(width, height, sizes)) {
            ALOGE("Video dimension (%dx%d) is unsupported", width, height);
 return BAD_VALUE;
 }
 if (isSetVideoSizeSupportedByCamera) {
            params->setVideoSize(width, height);
 } else {
            params->setPreviewSize(width, height);
 }
        isCameraParamChanged = true;
 } else if ((width == -1 && height != -1) ||
 (width != -1 && height == -1)) {
        ALOGE("Requested video size (%dx%d) is not supported", width, height);
 return BAD_VALUE;
 } else {  
 }

 if (frameRate != -1) {
        CHECK(frameRate > 0 && frameRate <= 120);
 const char* supportedFrameRates =
                params->get(CameraParameters::KEY_SUPPORTED_PREVIEW_FRAME_RATES);
        CHECK(supportedFrameRates != NULL);
        ALOGV("Supported frame rates: %s", supportedFrameRates);
 char buf[4];
        snprintf(buf, 4, "%d", frameRate);
 if (strstr(supportedFrameRates, buf) == NULL) {
            ALOGE("Requested frame rate (%d) is not supported: %s",
                frameRate, supportedFrameRates);
 return BAD_VALUE;
 }

        params->setPreviewFrameRate(frameRate);
        isCameraParamChanged = true;
 } else {  
 }

 if (isCameraParamChanged) {
 String8 s = params->flatten();
 if (OK != mCamera->setParameters(s)) {
            ALOGE("Could not change settings."
 " Someone else is using camera %p?", mCamera.get());
 return -EBUSY;
 }
 }
 return OK;
}
