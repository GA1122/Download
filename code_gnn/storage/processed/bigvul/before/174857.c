ICameraServiceListener::Status CameraService::getStatus(int cameraId) const {
 if (cameraId < 0 || cameraId >= MAX_CAMERAS) {
        ALOGE("%s: Invalid camera ID %d", __FUNCTION__, cameraId);
 return ICameraServiceListener::STATUS_UNKNOWN;
 }

 Mutex::Autolock al(mStatusMutex);
 return mStatusList[cameraId];
}
