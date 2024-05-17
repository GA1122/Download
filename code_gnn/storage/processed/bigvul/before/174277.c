status_t Camera3Device::submitRequestsHelper(
 const List<const CameraMetadata> &requests, bool repeating,
  
 int64_t *lastFrameNumber) {
    ATRACE_CALL();
 Mutex::Autolock il(mInterfaceLock);
 Mutex::Autolock l(mLock);

 status_t res = checkStatusOkToCaptureLocked();
 if (res != OK) {
 return res;
 }

 RequestList requestList;

    res = convertMetadataListToRequestListLocked(requests,  &requestList);
 if (res != OK) {
 return res;
 }

 if (repeating) {
        res = mRequestThread->setRepeatingRequests(requestList, lastFrameNumber);
 } else {
        res = mRequestThread->queueRequestList(requestList, lastFrameNumber);
 }

 if (res == OK) {
        waitUntilStateThenRelock( true, kActiveTimeout);
 if (res != OK) {
            SET_ERR_L("Can't transition to active in %f seconds!",
                    kActiveTimeout/1e9);
 }
        ALOGV("Camera %d: Capture request %" PRId32 " enqueued", mId,
 (*(requestList.begin()))->mResultExtras.requestId);
 } else {
        CLOGE("Cannot queue request. Impossible.");
 return BAD_VALUE;
 }

 return res;
}
