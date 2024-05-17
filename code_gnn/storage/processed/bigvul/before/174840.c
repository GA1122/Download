status_t CameraService::connectPro(
 const sp<IProCameraCallbacks>& cameraCb,
 int cameraId,
 const String16& clientPackageName,
 int clientUid,
  
                                        sp<IProCameraUser>& device)
{
 String8 clientName8(clientPackageName);
 int callingPid = getCallingPid();

    LOG1("CameraService::connectPro E (pid %d \"%s\", id %d)", callingPid,
            clientName8.string(), cameraId);
 status_t status = validateConnect(cameraId,  clientUid);
 if (status != OK) {
 return status;
 }

    sp<ProClient> client;
 {
 Mutex::Autolock lock(mServiceLock);
 {
            sp<BasicClient> client;
 if (!canConnectUnsafe(cameraId, clientPackageName,
                                  cameraCb->asBinder(),
  client)) {
 return -EBUSY;
 }
 }

 int facing = -1;
 int deviceVersion = getDeviceVersion(cameraId, &facing);

 switch(deviceVersion) {
 case CAMERA_DEVICE_API_VERSION_1_0:
            ALOGE("Camera id %d uses HALv1, doesn't support ProCamera",
                  cameraId);
 return -EOPNOTSUPP;
 break;
 case CAMERA_DEVICE_API_VERSION_2_0:
 case CAMERA_DEVICE_API_VERSION_2_1:
 case CAMERA_DEVICE_API_VERSION_3_0:
            client = new ProCamera2Client(this, cameraCb, clientPackageName,
                    cameraId, facing, callingPid, clientUid, getpid());
 break;
 case -1:
            ALOGE("Invalid camera id %d", cameraId);
 return BAD_VALUE;
 default:
            ALOGE("Unknown camera device HAL version: %d", deviceVersion);
 return INVALID_OPERATION;
 }

 status_t status = connectFinishUnsafe(client, client->getRemote());
 if (status != OK) {
 return status;
 }

        mProClientList[cameraId].push(client);

        LOG1("CameraService::connectPro X (id %d, this pid is %d)", cameraId,
                getpid());
 }
    device = client;
 return OK;
}
