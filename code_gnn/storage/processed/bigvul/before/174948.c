String8 CameraClient::getParameters() const {
 Mutex::Autolock lock(mLock);
 if (checkPidAndHardware() != NO_ERROR) return String8();

 String8 params(mHardware->getParameters().flatten());
    LOG1("getParameters (pid %d) (%s)", getCallingPid(), params.string());
 return params;
}
