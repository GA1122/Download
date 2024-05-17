CameraDeviceClient::CameraDeviceClient(const sp<CameraService>& cameraService,
 const sp<ICameraDeviceCallbacks>& remoteCallback,
 const String16& clientPackageName,
 int cameraId,
 int cameraFacing,
 int clientPid,
 uid_t clientUid,
 int servicePid) :
 Camera2ClientBase(cameraService, remoteCallback, clientPackageName,
                cameraId, cameraFacing, clientPid, clientUid, servicePid),
    mRequestIdCounter(0) {

    ATRACE_CALL();
    ALOGI("CameraDeviceClient %d: Opened", cameraId);
}
