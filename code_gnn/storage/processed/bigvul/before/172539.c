status_t StreamingProcessor::incrementStreamingIds() {
    ATRACE_CALL();
 Mutex::Autolock m(mMutex);

    mPreviewRequestId++;
 if (mPreviewRequestId >= Camera2Client::kPreviewRequestIdEnd) {
        mPreviewRequestId = Camera2Client::kPreviewRequestIdStart;
 }
    mRecordingRequestId++;
 if (mRecordingRequestId >= Camera2Client::kRecordingRequestIdEnd) {
        mRecordingRequestId = Camera2Client::kRecordingRequestIdStart;
 }
 return OK;
}
