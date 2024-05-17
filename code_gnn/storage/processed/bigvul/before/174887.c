status_t Camera2Client::commandEnableShutterSoundL(bool enable) {
 SharedParameters::Lock l(mParameters);
 if (enable) {
        l.mParameters.playShutterSound = true;
 return OK;
 }

 char value[PROPERTY_VALUE_MAX];
    property_get("ro.camera.sound.forced", value, "0");
 if (strncmp(value, "0", 2) != 0) {
 if (getCallingPid() != getpid()) {
            ALOGE("Failed to disable shutter sound. Permission denied (pid %d)",
                    getCallingPid());
 return PERMISSION_DENIED;
 }
 }

    l.mParameters.playShutterSound = false;
 return OK;
}
