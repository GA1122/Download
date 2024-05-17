Mutex* CameraService::getClientLockById(int cameraId) {
 if (cameraId < 0 || cameraId >= mNumberOfCameras) return NULL;
 return &mClientLock[cameraId];
}
