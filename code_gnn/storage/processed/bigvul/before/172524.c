void CameraSource::releaseCamera() {
    ALOGV("releaseCamera");
    sp<Camera> camera;
 bool coldCamera = false;
 {
 Mutex::Autolock autoLock(mLock);
        camera = mCamera;
        mCamera.clear();
        coldCamera = (mCameraFlags & FLAGS_HOT_CAMERA) == 0;
 }

 if (camera != 0) {
 int64_t token = IPCThreadState::self()->clearCallingIdentity();
 if (coldCamera) {
            ALOGV("Camera was cold when we started, stopping preview");
            camera->stopPreview();
            camera->disconnect();
 }
        camera->unlock();
 IPCThreadState::self()->restoreCallingIdentity(token);
 }

 {
 Mutex::Autolock autoLock(mLock);
 if (mCameraRecordingProxy != 0) {
 IInterface::asBinder(mCameraRecordingProxy)->unlinkToDeath(mDeathNotifier);
            mCameraRecordingProxy.clear();
 }
        mCameraFlags = 0;
 }
}
