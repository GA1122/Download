int CameraService::getDeviceVersion(int cameraId, int* facing) {
 struct camera_info info;
 if (mModule->get_camera_info(cameraId, &info) != OK) {
 return -1;
 }

 int deviceVersion;
 if (mModule->common.module_api_version >= CAMERA_MODULE_API_VERSION_2_0) {
        deviceVersion = info.device_version;
 } else {
        deviceVersion = CAMERA_DEVICE_API_VERSION_1_0;
 }

 if (facing) {
 *facing = info.facing;
 }

 return deviceVersion;
}
