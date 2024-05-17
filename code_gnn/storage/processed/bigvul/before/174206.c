status_t Camera3Device::convertMetadataListToRequestListLocked(
 const List<const CameraMetadata> &metadataList, RequestList *requestList) {
 if (requestList == NULL) {
        CLOGE("requestList cannot be NULL.");
 return BAD_VALUE;
 }

 int32_t burstId = 0;
 for (List<const CameraMetadata>::const_iterator it = metadataList.begin();
            it != metadataList.end(); ++it) {
        sp<CaptureRequest> newRequest = setUpRequestLocked(*it);
 if (newRequest == 0) {
            CLOGE("Can't create capture request");
 return BAD_VALUE;
 }

        newRequest->mResultExtras.burstId = burstId++;
 if (it->exists(ANDROID_REQUEST_ID)) {
 if (it->find(ANDROID_REQUEST_ID).count == 0) {
                CLOGE("RequestID entry exists; but must not be empty in metadata");
 return BAD_VALUE;
 }
            newRequest->mResultExtras.requestId = it->find(ANDROID_REQUEST_ID).data.i32[0];
 } else {
            CLOGE("RequestID does not exist in metadata");
 return BAD_VALUE;
 }

        requestList->push_back(newRequest);

        ALOGV("%s: requestId = %" PRId32, __FUNCTION__, newRequest->mResultExtras.requestId);
 }

 if (mIsConstrainedHighSpeedConfiguration && requestList->size() > 0) {
 auto firstRequest = requestList->begin();
 for (auto& outputStream : (*firstRequest)->mOutputStreams) {
 if (outputStream->isVideoStream()) {
 (*firstRequest)->mBatchSize = requestList->size();
 break;
 }
 }
 }

 return OK;
}
