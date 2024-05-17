status_t CameraClient::startRecordingMode() {
    LOG1("startRecordingMode");
 status_t result = NO_ERROR;

 if (mHardware->recordingEnabled()) {
 return NO_ERROR;
 }

 if (!mHardware->previewEnabled()) {
        result = startPreviewMode();
 if (result != NO_ERROR) {
 return result;
 }
 }

    enableMsgType(CAMERA_MSG_VIDEO_FRAME);
    mCameraService->playSound(CameraService::SOUND_RECORDING);
    result = mHardware->startRecording();
 if (result != NO_ERROR) {
        ALOGE("mHardware->startRecording() failed with status %d", result);
 }
 return result;
}
