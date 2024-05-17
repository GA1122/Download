CameraClient::CameraClient(const sp<CameraService>& cameraService,
 const sp<ICameraClient>& cameraClient,
 const String16& clientPackageName,
 int cameraId, int cameraFacing,
 int clientPid, int clientUid,
 int servicePid):
 Client(cameraService, cameraClient, clientPackageName,
                cameraId, cameraFacing, clientPid, clientUid, servicePid)
{
 int callingPid = getCallingPid();
    LOG1("CameraClient::CameraClient E (pid %d, id %d)", callingPid, cameraId);

    mHardware = NULL;
    mMsgEnabled = 0;
    mSurface = 0;
    mPreviewWindow = 0;
    mDestructionStarted = false;

    mPreviewCallbackFlag = CAMERA_FRAME_CALLBACK_FLAG_NOOP;
    mOrientation = getOrientation(0, mCameraFacing == CAMERA_FACING_FRONT);
    mPlayShutterSound = true;
    LOG1("CameraClient::CameraClient X (pid %d, id %d)", callingPid, cameraId);
}
