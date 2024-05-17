status_t Camera3Device::RequestThread::clearRepeatingRequests( int64_t *lastFrameNumber) {
 Mutex::Autolock l(mRequestLock);
    mRepeatingRequests.clear();
 if (lastFrameNumber != NULL) {
 *lastFrameNumber = mRepeatingLastFrameNumber;
 }
    mRepeatingLastFrameNumber = NO_IN_FLIGHT_REPEATING_FRAMES;
 return OK;
}
