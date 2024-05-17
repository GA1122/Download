CameraService::Client::Client(const sp<CameraService>& cameraService,
 const sp<ICameraClient>& cameraClient,
 const String16& clientPackageName,
 int cameraId, int cameraFacing,
 int clientPid, uid_t clientUid,
 int servicePid) :
 CameraService::BasicClient(cameraService, cameraClient->asBinder(),
                clientPackageName,
                cameraId, cameraFacing,
                clientPid, clientUid,
                servicePid)
{
 int callingPid = getCallingPid();
    LOG1("Client::Client E (pid %d, id %d)", callingPid, cameraId);

    mRemoteCallback = cameraClient;

    cameraService->setCameraBusy(cameraId);
    cameraService->loadSound();

    LOG1("Client::Client X (pid %d, id %d)", callingPid, cameraId);
}
