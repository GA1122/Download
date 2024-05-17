status_t CameraService::connect(
 const sp<ICameraClient>& cameraClient,
 int cameraId,
 const String16& clientPackageName,
 int clientUid,
  
        sp<ICamera>& device) {

 String8 clientName8(clientPackageName);
 int callingPid = getCallingPid();

    LOG1("CameraService::connect E (pid %d \"%s\", id %d)", callingPid,
            clientName8.string(), cameraId);

 status_t status = validateConnect(cameraId,  clientUid);
 if (status != OK) {
 return status;
 }


    sp<Client> client;
 {
 Mutex::Autolock lock(mServiceLock);
        sp<BasicClient> clientTmp;
 if (!canConnectUnsafe(cameraId, clientPackageName,
                              cameraClient->asBinder(),
  clientTmp)) {
 return -EBUSY;
 } else if (client.get() != NULL) {
            device = static_cast<Client*>(clientTmp.get());
 return OK;
 }

 int facing = -1;
 int deviceVersion = getDeviceVersion(cameraId, &facing);

 if (isValidCameraId(cameraId)) {
            updateStatus(ICameraServiceListener::STATUS_NOT_AVAILABLE,
                         cameraId);
 }

 switch(deviceVersion) {
 case CAMERA_DEVICE_API_VERSION_1_0:
            client = new CameraClient(this, cameraClient,
                    clientPackageName, cameraId,
                    facing, callingPid, clientUid, getpid());
 break;
 case CAMERA_DEVICE_API_VERSION_2_0:
 case CAMERA_DEVICE_API_VERSION_2_1:
 case CAMERA_DEVICE_API_VERSION_3_0:
            client = new Camera2Client(this, cameraClient,
                    clientPackageName, cameraId,
                    facing, callingPid, clientUid, getpid(),
                    deviceVersion);
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
            updateStatus(ICameraServiceListener::STATUS_PRESENT, cameraId);
 return status;
 }

        mClient[cameraId] = client;
        LOG1("CameraService::connect X (id %d, this pid is %d)", cameraId,
             getpid());
 }

    device = client;
 return OK;
}
