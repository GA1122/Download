status_t Camera3Device::dump(int fd, const Vector<String16> &args) {
    ATRACE_CALL();
 (void)args;

 bool gotInterfaceLock = tryLockSpinRightRound(mInterfaceLock);
 bool gotLock = tryLockSpinRightRound(mLock);

    ALOGW_IF(!gotInterfaceLock,
 "Camera %d: %s: Unable to lock interface lock, proceeding anyway",
            mId, __FUNCTION__);
    ALOGW_IF(!gotLock,
 "Camera %d: %s: Unable to lock main lock, proceeding anyway",
            mId, __FUNCTION__);

 String8 lines;

 const char *status =
            mStatus == STATUS_ERROR         ? "ERROR" :
            mStatus == STATUS_UNINITIALIZED ? "UNINITIALIZED" :
            mStatus == STATUS_UNCONFIGURED  ? "UNCONFIGURED" :
            mStatus == STATUS_CONFIGURED    ? "CONFIGURED" :
            mStatus == STATUS_ACTIVE        ? "ACTIVE" :
 "Unknown";

    lines.appendFormat("    Device status: %s\n", status);
 if (mStatus == STATUS_ERROR) {
        lines.appendFormat("    Error cause: %s\n", mErrorCause.string());
 }
    lines.appendFormat("    Stream configuration:\n");
    lines.appendFormat("    Operation mode: %s \n", mIsConstrainedHighSpeedConfiguration ?
 "CONSTRAINED HIGH SPEED VIDEO" : "NORMAL");

 if (mInputStream != NULL) {
        write(fd, lines.string(), lines.size());
        mInputStream->dump(fd, args);
 } else {
        lines.appendFormat("      No input stream.\n");
        write(fd, lines.string(), lines.size());
 }
 for (size_t i = 0; i < mOutputStreams.size(); i++) {
        mOutputStreams[i]->dump(fd,args);
 }

    lines = String8("    In-flight requests:\n");
 if (mInFlightMap.size() == 0) {
        lines.append("      None\n");
 } else {
 for (size_t i = 0; i < mInFlightMap.size(); i++) {
 InFlightRequest r = mInFlightMap.valueAt(i);
            lines.appendFormat("      Frame %d |  Timestamp: %" PRId64 ", metadata"
 " arrived: %s, buffers left: %d\n", mInFlightMap.keyAt(i),
                    r.shutterTimestamp, r.haveResultMetadata ? "true" : "false",
                    r.numBuffersLeft);
 }
 }
    write(fd, lines.string(), lines.size());

 {
        lines = String8("    Last request sent:\n");
        write(fd, lines.string(), lines.size());

 CameraMetadata lastRequest = getLatestRequestLocked();
        lastRequest.dump(fd,  2,  6);
 }

 if (mHal3Device != NULL) {
        lines = String8("    HAL device dump:\n");
        write(fd, lines.string(), lines.size());
        mHal3Device->ops->dump(mHal3Device, fd);
 }

 if (gotLock) mLock.unlock();
 if (gotInterfaceLock) mInterfaceLock.unlock();

 return OK;
}
