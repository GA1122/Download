CameraService::BasicClient::BasicClient(const sp<CameraService>& cameraService,
 const sp<IBinder>& remoteCallback,
 const String16& clientPackageName,
 int cameraId, int cameraFacing,
 int clientPid, uid_t clientUid,
 int servicePid):
        mClientPackageName(clientPackageName)
{
    mCameraService = cameraService;
    mRemoteBinder = remoteCallback;
    mCameraId = cameraId;
    mCameraFacing = cameraFacing;
    mClientPid = clientPid;
    mClientUid = clientUid;
    mServicePid = servicePid;
    mOpsActive = false;
    mDestructionStarted = false;
}
