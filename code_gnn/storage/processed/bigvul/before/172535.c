int32_t StreamingProcessor::getActiveRequestId() const {
 Mutex::Autolock m(mMutex);
 switch (mActiveRequest) {
 case NONE:
 return 0;
 case PREVIEW:
 return mPreviewRequestId;
 case RECORD:
 return mRecordingRequestId;
 default:
            ALOGE("%s: Unexpected mode %d", __FUNCTION__, mActiveRequest);
 return 0;
 }
}
