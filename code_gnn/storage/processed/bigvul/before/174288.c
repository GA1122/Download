void Camera3Device::RequestThread::waitForNextRequestBatch() {
 Mutex::Autolock l(mRequestLock);

    assert(mNextRequests.empty());

 NextRequest nextRequest;
    nextRequest.captureRequest = waitForNextRequestLocked();
 if (nextRequest.captureRequest == nullptr) {
 return;
 }

    nextRequest.halRequest = camera3_capture_request_t();
    nextRequest.submitted = false;
    mNextRequests.add(nextRequest);

 const size_t batchSize = nextRequest.captureRequest->mBatchSize;

 for (size_t i = 1; i < batchSize; i++) {
 NextRequest additionalRequest;
        additionalRequest.captureRequest = waitForNextRequestLocked();
 if (additionalRequest.captureRequest == nullptr) {
 break;
 }

        additionalRequest.halRequest = camera3_capture_request_t();
        additionalRequest.submitted = false;
        mNextRequests.add(additionalRequest);
 }

 if (mNextRequests.size() < batchSize) {
        ALOGE("RequestThread: only get %d out of %d requests. Skipping requests.",
                mNextRequests.size(), batchSize);
        cleanUpFailedRequests( true);
 }

 return;
}
