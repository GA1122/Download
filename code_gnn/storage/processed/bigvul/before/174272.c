status_t Camera3Device::RequestThread::setRepeatingRequests(
 const RequestList &requests,
  
 int64_t *lastFrameNumber) {
 Mutex::Autolock l(mRequestLock);
 if (lastFrameNumber != NULL) {
 *lastFrameNumber = mRepeatingLastFrameNumber;
 }
    mRepeatingRequests.clear();
    mRepeatingRequests.insert(mRepeatingRequests.begin(),
            requests.begin(), requests.end());

    unpauseForNewRequests();

    mRepeatingLastFrameNumber = NO_IN_FLIGHT_REPEATING_FRAMES;
 return OK;
}
