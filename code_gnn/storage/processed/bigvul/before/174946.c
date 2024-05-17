status_t CameraClient::enableShutterSound(bool enable) {
    LOG1("enableShutterSound (pid %d)", getCallingPid());

 status_t result = checkPidAndHardware();
 if (result != NO_ERROR) return result;

 if (enable) {
        mPlayShutterSound = true;
 return OK;
 }

 char value[PROPERTY_VALUE_MAX];
    property_get("ro.camera.sound.forced", value, "0");
 if (strcmp(value, "0") != 0) {
 if (getCallingPid() != getpid()) {
            ALOGE("Failed to disable shutter sound. Permission denied (pid %d)", getCallingPid());
 return PERMISSION_DENIED;
 }
 }

    mPlayShutterSound = false;
 return OK;
}
