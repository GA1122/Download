sp<Camera3Device::CaptureRequest> Camera3Device::createCaptureRequest(
 const CameraMetadata &request) {
    ATRACE_CALL();
 status_t res;

    sp<CaptureRequest> newRequest = new CaptureRequest;
    newRequest->mSettings = request;

 camera_metadata_entry_t inputStreams =
            newRequest->mSettings.find(ANDROID_REQUEST_INPUT_STREAMS);
 if (inputStreams.count > 0) {
 if (mInputStream == NULL ||
                mInputStream->getId() != inputStreams.data.i32[0]) {
            CLOGE("Request references unknown input stream %d",
                    inputStreams.data.u8[0]);
 return NULL;
 }
 if (mInputStream->isConfiguring()) {
            res = mInputStream->finishConfiguration(mHal3Device);
 if (res != OK) {
                SET_ERR_L("Unable to finish configuring input stream %d:"
 " %s (%d)",
                        mInputStream->getId(), strerror(-res), res);
 return NULL;
 }
 }
 if (mInputStream->isPreparing()) {
            CLOGE("Request references an input stream that's being prepared!");
 return NULL;
 }

        newRequest->mInputStream = mInputStream;
        newRequest->mSettings.erase(ANDROID_REQUEST_INPUT_STREAMS);
 }

 camera_metadata_entry_t streams =
            newRequest->mSettings.find(ANDROID_REQUEST_OUTPUT_STREAMS);
 if (streams.count == 0) {
        CLOGE("Zero output streams specified!");
 return NULL;
 }

 for (size_t i = 0; i < streams.count; i++) {
 int idx = mOutputStreams.indexOfKey(streams.data.i32[i]);
 if (idx == NAME_NOT_FOUND) {
            CLOGE("Request references unknown stream %d",
                    streams.data.u8[i]);
 return NULL;
 }
        sp<Camera3OutputStreamInterface> stream =
                mOutputStreams.editValueAt(idx);

 if (stream->isConfiguring()) {
            res = stream->finishConfiguration(mHal3Device);
 if (res != OK) {
                SET_ERR_L("Unable to finish configuring stream %d: %s (%d)",
                        stream->getId(), strerror(-res), res);
 return NULL;
 }
 }
 if (stream->isPreparing()) {
            CLOGE("Request references an output stream that's being prepared!");
 return NULL;
 }

        newRequest->mOutputStreams.push(stream);
 }
    newRequest->mSettings.erase(ANDROID_REQUEST_OUTPUT_STREAMS);
    newRequest->mBatchSize = 1;

 return newRequest;
}
