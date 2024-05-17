bool CameraService::isValidCameraId(int cameraId) {
 int facing;
 int deviceVersion = getDeviceVersion(cameraId, &facing);

 switch(deviceVersion) {
 case CAMERA_DEVICE_API_VERSION_1_0:
 case CAMERA_DEVICE_API_VERSION_2_0:
 case CAMERA_DEVICE_API_VERSION_2_1:
 case CAMERA_DEVICE_API_VERSION_3_0:
 return true;
 default:
 return false;
 }

 return false;
}
