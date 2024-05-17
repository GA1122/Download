StreamingProcessor::StreamingProcessor(sp<Camera2Client> client):
        mClient(client),
        mDevice(client->getCameraDevice()),
        mId(client->getCameraId()),
        mActiveRequest(NONE),
        mPaused(false),
        mPreviewRequestId(Camera2Client::kPreviewRequestIdStart),
        mPreviewStreamId(NO_STREAM),
        mRecordingRequestId(Camera2Client::kRecordingRequestIdStart),
        mRecordingStreamId(NO_STREAM),
        mRecordingFrameAvailable(false),
        mRecordingHeapCount(kDefaultRecordingHeapCount),
        mRecordingHeapFree(kDefaultRecordingHeapCount),
        mRecordingFormat(kDefaultRecordingFormat),
        mRecordingDataSpace(kDefaultRecordingDataSpace),
        mRecordingGrallocUsage(kDefaultRecordingGrallocUsage)
{
}
