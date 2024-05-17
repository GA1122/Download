status_t CameraService::connectDevice(
 const sp<ICameraDeviceCallbacks>& cameraCb,
 int cameraId,
 const String16& clientPackageName,
 int clientUid,
  
        sp<ICameraDeviceUser>& device)
{

 String8 clientName8(clientPackageName);
 int callingPid = getCallingPid();

    LOG1("CameraService::connectDevice E (pid %d \"%s\", id %d)", callingPid,
            clientName8.string(), cameraId);

 status_t status = validateConnect(cameraId,  clientUid);
 if (status != OK) {
 return status;
 }

    sp<CameraDeviceClient> client;
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

 if (isValidCameraId(cameraId)) {
            updateStatus(ICameraServiceListener::STATUS_NOT_AVAILABLE,
                         cameraId);
 }

 switch(deviceVersion) {
 case CAMERA_DEVICE_API_VERSION_1_0:
            ALOGW("Camera using old HAL version: %d", deviceVersion);
 return -EOPNOTSUPP;
 case CAMERA_DEVICE_API_VERSION_2_0:
 case CAMERA_DEVICE_API_VERSION_2_1:
 case CAMERA_DEVICE_API_VERSION_3_0:
            client = new CameraDeviceClient(this, cameraCb, clientPackageName,
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
            updateStatus(ICameraServiceListener::STATUS_PRESENT, cameraId);
 return status;
 }

        LOG1("CameraService::connectDevice X (id %d, this pid is %d)", cameraId,
                getpid());

        mClient[cameraId] = client;
 }

    device = client;
 return OK;
}
