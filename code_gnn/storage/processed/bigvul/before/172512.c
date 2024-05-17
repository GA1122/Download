static void getSupportedVideoSizes(
 const CameraParameters& params,
 bool *isSetVideoSizeSupported,
 Vector<Size>& sizes) {

 *isSetVideoSizeSupported = true;
    params.getSupportedVideoSizes(sizes);
 if (sizes.size() == 0) {
        ALOGD("Camera does not support setVideoSize()");
        params.getSupportedPreviewSizes(sizes);
 *isSetVideoSizeSupported = false;
 }
}
