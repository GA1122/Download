status_t CameraService::validateConnect(int cameraId,
  
 int& clientUid) const {

 int callingPid = getCallingPid();

 if (clientUid == USE_CALLING_UID) {
        clientUid = getCallingUid();
 } else {
 if (callingPid != getpid()) {
            ALOGE("CameraService::connect X (pid %d) rejected (don't trust clientUid)",
                    callingPid);
 return PERMISSION_DENIED;
 }
 }

 if (!mModule) {
        ALOGE("Camera HAL module not loaded");
 return -ENODEV;
 }

 if (cameraId < 0 || cameraId >= mNumberOfCameras) {
        ALOGE("CameraService::connect X (pid %d) rejected (invalid cameraId %d).",
            callingPid, cameraId);
 return -ENODEV;
 }

 char value[PROPERTY_VALUE_MAX];
    property_get("sys.secpolicy.camera.disabled", value, "0");
 if (strcmp(value, "1") == 0) {
        ALOGI("Camera is disabled. connect X (pid %d) rejected", callingPid);
 return -EACCES;
 }

 ICameraServiceListener::Status currentStatus = getStatus(cameraId);
 if (currentStatus == ICameraServiceListener::STATUS_NOT_PRESENT) {
        ALOGI("Camera is not plugged in,"
 " connect X (pid %d) rejected", callingPid);
 return -ENODEV;
 } else if (currentStatus == ICameraServiceListener::STATUS_ENUMERATING) {
        ALOGI("Camera is enumerating,"
 " connect X (pid %d) rejected", callingPid);
 return -EBUSY;
 }

 return OK;
}
