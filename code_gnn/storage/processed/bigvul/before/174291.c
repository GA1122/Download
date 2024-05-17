status_t Camera3Device::waitUntilDrained() {
    ATRACE_CALL();
 Mutex::Autolock il(mInterfaceLock);
 Mutex::Autolock l(mLock);

 return waitUntilDrainedLocked();
}
