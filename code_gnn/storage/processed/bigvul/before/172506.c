status_t CameraSource::checkFrameRate(
 const CameraParameters& params,
 int32_t frameRate) {

    ALOGV("checkFrameRate");
 int32_t frameRateActual = params.getPreviewFrameRate();
 if (frameRateActual < 0) {
        ALOGE("Failed to retrieve preview frame rate (%d)", frameRateActual);
 return UNKNOWN_ERROR;
 }

 if (frameRate != -1 && (frameRateActual - frameRate) != 0) {
        ALOGE("Failed to set preview frame rate to %d fps. The actual "
 "frame rate is %d", frameRate, frameRateActual);
 return UNKNOWN_ERROR;
 }

    mVideoFrameRate = frameRateActual;
 return OK;
}
