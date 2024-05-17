status_t Camera3Device::RequestThread::waitUntilRequestProcessed(
 int32_t requestId, nsecs_t timeout) {
 Mutex::Autolock l(mLatestRequestMutex);
 status_t res;
 while (mLatestRequestId != requestId) {
 nsecs_t startTime = systemTime();

        res = mLatestRequestSignal.waitRelative(mLatestRequestMutex, timeout);
 if (res != OK) return res;

        timeout -= (systemTime() - startTime);
 }

 return OK;
}
