status_t CameraClient::lock() {
 int callingPid = getCallingPid();
    LOG1("lock (pid %d)", callingPid);
 Mutex::Autolock lock(mLock);

 if (mClientPid == 0) {
        mClientPid = callingPid;
 return NO_ERROR;
 }

 return checkPid();
}
