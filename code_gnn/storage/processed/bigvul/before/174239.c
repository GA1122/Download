bool Camera3Device::RequestThread::isRepeatingRequestLocked(const sp<CaptureRequest> requestIn) {
 if (mRepeatingRequests.empty()) {
 return false;
 }
 int32_t requestId = requestIn->mResultExtras.requestId;
 const RequestList &repeatRequests = mRepeatingRequests;
 const sp<CaptureRequest> firstRequest = *repeatRequests.begin();
 return (firstRequest->mResultExtras.requestId == requestId);
}
