status_t CameraSource::isCameraColorFormatSupported(
 const CameraParameters& params) {
    mColorFormat = getColorFormat(params.get(
 CameraParameters::KEY_VIDEO_FRAME_FORMAT));
 if (mColorFormat == -1) {
 return BAD_VALUE;
 }
 return OK;
}
