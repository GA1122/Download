void CameraSource::stopCameraRecording() {
    ALOGV("stopCameraRecording");
 if (mCameraFlags & FLAGS_HOT_CAMERA) {
        mCameraRecordingProxy->stopRecording();
 } else {
        mCamera->setListener(NULL);
        mCamera->stopRecording();
 }
}
