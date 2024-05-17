CameraService::BasicClient* CameraService::getClientByIdUnsafe(int cameraId) {
 if (cameraId < 0 || cameraId >= mNumberOfCameras) return NULL;
 return mClient[cameraId].unsafe_get();
}
