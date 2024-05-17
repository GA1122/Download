status_t CameraSource::isCameraAvailable(
 const sp<ICamera>& camera, const sp<ICameraRecordingProxy>& proxy,
 int32_t cameraId, const String16& clientName, uid_t clientUid) {

 if (camera == 0) {
        mCamera = Camera::connect(cameraId, clientName, clientUid);
 if (mCamera == 0) return -EBUSY;
        mCameraFlags &= ~FLAGS_HOT_CAMERA;
 } else {
        mCamera = Camera::create(camera);
 if (mCamera == 0) return -EBUSY;
        mCameraRecordingProxy = proxy;
        mCameraFlags |= FLAGS_HOT_CAMERA;
        mDeathNotifier = new DeathNotifier();
 IInterface::asBinder(mCameraRecordingProxy)->linkToDeath(mDeathNotifier);
 }

    mCamera->lock();

 return OK;
}
