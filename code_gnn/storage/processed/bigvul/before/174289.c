 Camera3Device::RequestThread::waitForNextRequestLocked() {
 status_t res;
    sp<CaptureRequest> nextRequest;

 while (mRequestQueue.empty()) {
 if (!mRepeatingRequests.empty()) {
 const RequestList &requests = mRepeatingRequests;
 RequestList::const_iterator firstRequest =
                    requests.begin();
            nextRequest = *firstRequest;
            mRequestQueue.insert(mRequestQueue.end(),
 ++firstRequest,
                    requests.end());

            mRepeatingLastFrameNumber = mFrameNumber + requests.size() - 1;

 break;
 }

        res = mRequestSignal.waitRelative(mRequestLock, kRequestTimeout);

 if ((mRequestQueue.empty() && mRepeatingRequests.empty()) ||
                exitPending()) {
 Mutex::Autolock pl(mPauseLock);
 if (mPaused == false) {
                ALOGV("%s: RequestThread: Going idle", __FUNCTION__);
                mPaused = true;
                sp<StatusTracker> statusTracker = mStatusTracker.promote();
 if (statusTracker != 0) {
                    statusTracker->markComponentIdle(mStatusId, Fence::NO_FENCE);
 }
 }
 return NULL;
 }
 }

 if (nextRequest == NULL) {
 RequestList::iterator firstRequest =
                mRequestQueue.begin();
        nextRequest = *firstRequest;
        mRequestQueue.erase(firstRequest);
 }

 Mutex::Autolock pl(mPauseLock);
 if (mPaused) {
        ALOGV("%s: RequestThread: Unpaused", __FUNCTION__);
        sp<StatusTracker> statusTracker = mStatusTracker.promote();
 if (statusTracker != 0) {
            statusTracker->markComponentActive(mStatusId);
 }
 }
    mPaused = false;

 if (mReconfigured) {
        mPrevRequest.clear();
        mReconfigured = false;
 }

 if (nextRequest != NULL) {
        nextRequest->mResultExtras.frameNumber = mFrameNumber++;
        nextRequest->mResultExtras.afTriggerId = mCurrentAfTriggerId;
        nextRequest->mResultExtras.precaptureTriggerId = mCurrentPreCaptureTriggerId;

 if (nextRequest->mInputStream != NULL) {
            res = nextRequest->mInputStream->getInputBuffer(&nextRequest->mInputBuffer);
 if (res != OK) {
                ALOGE("%s: Can't get input buffer, skipping request:"
 " %s (%d)", __FUNCTION__, strerror(-res), res);
 if (mListener != NULL) {
                    mListener->notifyError(
 ICameraDeviceCallbacks::ERROR_CAMERA_REQUEST,
                            nextRequest->mResultExtras);
 }
 return NULL;
 }
 }
 }

    handleAePrecaptureCancelRequest(nextRequest);

 return nextRequest;
}
