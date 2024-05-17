void CameraService::onDeviceStatusChanged(int cameraId,
 int newStatus)
{
    ALOGI("%s: Status changed for cameraId=%d, newStatus=%d", __FUNCTION__,
          cameraId, newStatus);

 if (cameraId < 0 || cameraId >= MAX_CAMERAS) {
        ALOGE("%s: Bad camera ID %d", __FUNCTION__, cameraId);
 return;
 }

 if ((int)getStatus(cameraId) == newStatus) {
        ALOGE("%s: State transition to the same status 0x%x not allowed",
              __FUNCTION__, (uint32_t)newStatus);
 return;
 }

  
 if (newStatus == CAMERA_DEVICE_STATUS_NOT_PRESENT) {
 Vector<sp<BasicClient> > clientsToDisconnect;
 {
 Mutex::Autolock al(mServiceLock);

  
           sp<BasicClient> client = mClient[cameraId].promote();
 if (client.get() != NULL) {
               clientsToDisconnect.push_back(client);
 }

 int i = cameraId;
 for (size_t j = 0; j < mProClientList[i].size(); ++j) {
               sp<ProClient> cl = mProClientList[i][j].promote();
 if (cl != NULL) {
                   clientsToDisconnect.push_back(cl);
 }
 }
 }

  

 for (size_t i = 0; i < clientsToDisconnect.size(); ++i) {
            sp<BasicClient> client = clientsToDisconnect[i];

            client->disconnect();
  
 }

        ALOGV("%s: After unplug, disconnected %d clients",
              __FUNCTION__, clientsToDisconnect.size());
 }

    updateStatus(
 static_cast<ICameraServiceListener::Status>(newStatus), cameraId);

}
