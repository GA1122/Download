status_t CameraClient::checkPidAndHardware() const {
 status_t result = checkPid();
 if (result != NO_ERROR) return result;
 if (mHardware == 0) {
        ALOGE("attempt to use a camera after disconnect() (pid %d)", getCallingPid());
 return INVALID_OPERATION;
 }
 return NO_ERROR;
}
