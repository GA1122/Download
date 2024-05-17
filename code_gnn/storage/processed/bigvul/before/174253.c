status_t Camera3Device::RequestThread::queueRequestList(
 List<sp<CaptureRequest> > &requests,
  
 int64_t *lastFrameNumber) {
 Mutex::Autolock l(mRequestLock);
 for (List<sp<CaptureRequest> >::iterator it = requests.begin(); it != requests.end();
 ++it) {
        mRequestQueue.push_back(*it);
 }

 if (lastFrameNumber != NULL) {
 *lastFrameNumber = mFrameNumber + mRequestQueue.size() - 1;
        ALOGV("%s: requestId %d, mFrameNumber %" PRId32 ", lastFrameNumber %" PRId64 ".",
              __FUNCTION__, (*(requests.begin()))->mResultExtras.requestId, mFrameNumber,
 *lastFrameNumber);
 }

    unpauseForNewRequests();

 return OK;
}
