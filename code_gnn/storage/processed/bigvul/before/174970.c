status_t CameraClient::startCameraMode(camera_mode mode) {
    LOG1("startCameraMode(%d)", mode);
 Mutex::Autolock lock(mLock);
 status_t result = checkPidAndHardware();
 if (result != NO_ERROR) return result;

 switch(mode) {
 case CAMERA_PREVIEW_MODE:
 if (mSurface == 0 && mPreviewWindow == 0) {
                LOG1("mSurface is not set yet.");
 }
 return startPreviewMode();
 case CAMERA_RECORDING_MODE:
 if (mSurface == 0 && mPreviewWindow == 0) {
                ALOGE("mSurface or mPreviewWindow must be set before startRecordingMode.");
 return INVALID_OPERATION;
 }
 return startRecordingMode();
 default:
 return UNKNOWN_ERROR;
 }
}
