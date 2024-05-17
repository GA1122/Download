status_t CameraSource::checkVideoSize(
 const CameraParameters& params,
 int32_t width, int32_t height) {

    ALOGV("checkVideoSize");
 int32_t frameWidthActual = -1;
 int32_t frameHeightActual = -1;
 Vector<Size> sizes;
    params.getSupportedVideoSizes(sizes);
 if (sizes.size() == 0) {
        params.getPreviewSize(&frameWidthActual, &frameHeightActual);
 } else {
        params.getVideoSize(&frameWidthActual, &frameHeightActual);
 }
 if (frameWidthActual < 0 || frameHeightActual < 0) {
        ALOGE("Failed to retrieve video frame size (%dx%d)",
                frameWidthActual, frameHeightActual);
 return UNKNOWN_ERROR;
 }

 if (width != -1 && height != -1) {
 if (frameWidthActual != width || frameHeightActual != height) {
            ALOGE("Failed to set video frame size to %dx%d. "
 "The actual video size is %dx%d ", width, height,
                    frameWidthActual, frameHeightActual);
 return UNKNOWN_ERROR;
 }
 }

    mVideoSize.width = frameWidthActual;
    mVideoSize.height = frameHeightActual;
 return OK;
}
