status_t CameraService::getCameraInfo(int cameraId,
 struct CameraInfo* cameraInfo) {
 if (!mModule) {
 return -ENODEV;
 }

 if (cameraId < 0 || cameraId >= mNumberOfCameras) {
 return BAD_VALUE;
 }

 struct camera_info info;
 status_t rc = mModule->get_camera_info(cameraId, &info);
    cameraInfo->facing = info.facing;
    cameraInfo->orientation = info.orientation;
 return rc;
}
