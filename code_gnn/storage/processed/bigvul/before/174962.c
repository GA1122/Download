bool CameraClient::recordingEnabled() {
    LOG1("recordingEnabled (pid %d)", getCallingPid());

 Mutex::Autolock lock(mLock);
 if (checkPidAndHardware() != NO_ERROR) return false;
 return mHardware->recordingEnabled();
}
