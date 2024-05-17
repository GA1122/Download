ProCamera2Client::ProCamera2Client(const sp<CameraService>& cameraService,
 const sp<IProCameraCallbacks>& remoteCallback,
 const String16& clientPackageName,
 int cameraId,
 int cameraFacing,
 int clientPid,
 uid_t clientUid,
 int servicePid) :
 Camera2ClientBase(cameraService, remoteCallback, clientPackageName,
                cameraId, cameraFacing, clientPid, clientUid, servicePid)
{
    ATRACE_CALL();
    ALOGI("ProCamera %d: Opened", cameraId);

    mExclusiveLock = false;
}
