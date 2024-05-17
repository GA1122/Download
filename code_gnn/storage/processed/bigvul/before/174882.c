Camera2Client::Camera2Client(const sp<CameraService>& cameraService,
 const sp<ICameraClient>& cameraClient,
 const String16& clientPackageName,
 int cameraId,
 int cameraFacing,
 int clientPid,
 uid_t clientUid,
 int servicePid,
 int deviceVersion):
 Camera2ClientBase(cameraService, cameraClient, clientPackageName,
                cameraId, cameraFacing, clientPid, clientUid, servicePid),
        mParameters(cameraId, cameraFacing),
        mDeviceVersion(deviceVersion)
{
    ATRACE_CALL();

 SharedParameters::Lock l(mParameters);
    l.mParameters.state = Parameters::DISCONNECTED;
}
