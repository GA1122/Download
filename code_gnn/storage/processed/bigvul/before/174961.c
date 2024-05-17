bool CameraClient::previewEnabled() {
    LOG1("previewEnabled (pid %d)", getCallingPid());

 Mutex::Autolock lock(mLock);
 if (checkPidAndHardware() != NO_ERROR) return false;
 return mHardware->previewEnabled();
}
