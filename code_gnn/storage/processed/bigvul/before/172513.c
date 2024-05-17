status_t CameraSource::init(
 const sp<ICamera>& camera,
 const sp<ICameraRecordingProxy>& proxy,
 int32_t cameraId,
 const String16& clientName,
 uid_t clientUid,
 Size videoSize,
 int32_t frameRate,
 bool storeMetaDataInVideoBuffers) {

    ALOGV("init");
 status_t err = OK;
 int64_t token = IPCThreadState::self()->clearCallingIdentity();
    err = initWithCameraAccess(camera, proxy, cameraId, clientName, clientUid,
                               videoSize, frameRate,
                               storeMetaDataInVideoBuffers);
 IPCThreadState::self()->restoreCallingIdentity(token);
 return err;
}
