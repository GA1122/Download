status_t CameraClient::cancelAutoFocus() {
    LOG1("cancelAutoFocus (pid %d)", getCallingPid());

 Mutex::Autolock lock(mLock);
 status_t result = checkPidAndHardware();
 if (result != NO_ERROR) return result;

 return mHardware->cancelAutoFocus();
}
