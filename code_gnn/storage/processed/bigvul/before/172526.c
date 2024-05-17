void CameraSource::releaseRecordingFrame(const sp<IMemory>& frame) {
    ALOGV("releaseRecordingFrame");
 if (mCameraRecordingProxy != NULL) {
        mCameraRecordingProxy->releaseRecordingFrame(frame);
 } else if (mCamera != NULL) {
 int64_t token = IPCThreadState::self()->clearCallingIdentity();
        mCamera->releaseRecordingFrame(frame);
 IPCThreadState::self()->restoreCallingIdentity(token);
 }
}
