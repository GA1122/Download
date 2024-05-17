CameraService::ProClient::ProClient(const sp<CameraService>& cameraService,
 const sp<IProCameraCallbacks>& remoteCallback,
 const String16& clientPackageName,
 int cameraId,
 int cameraFacing,
 int clientPid,
 uid_t clientUid,
 int servicePid)
 : CameraService::BasicClient(cameraService, remoteCallback->asBinder(),
                clientPackageName, cameraId, cameraFacing,
                clientPid,  clientUid, servicePid)
{
    mRemoteCallback = remoteCallback;
}
