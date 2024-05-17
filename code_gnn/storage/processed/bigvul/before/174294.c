status_t Camera3Device::waitUntilRequestReceived(int32_t requestId, nsecs_t timeout) {
    ATRACE_CALL();
 Mutex::Autolock il(mInterfaceLock);

 return mRequestThread->waitUntilRequestProcessed(requestId, timeout);
}
