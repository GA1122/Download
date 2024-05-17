status_t Camera3Device::waitForNextFrame(nsecs_t timeout) {
 status_t res;
 Mutex::Autolock l(mOutputLock);

 while (mResultQueue.empty()) {
        res = mResultSignal.waitRelative(mOutputLock, timeout);
 if (res == TIMED_OUT) {
 return res;
 } else if (res != OK) {
            ALOGW("%s: Camera %d: No frame in %" PRId64 " ns: %s (%d)",
                    __FUNCTION__, mId, timeout, strerror(-res), res);
 return res;
 }
 }
 return OK;
}
