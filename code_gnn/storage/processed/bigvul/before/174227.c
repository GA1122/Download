status_t Camera3Device::getNextResult(CaptureResult *frame) {
    ATRACE_CALL();
 Mutex::Autolock l(mOutputLock);

 if (mResultQueue.empty()) {
 return NOT_ENOUGH_DATA;
 }

 if (frame == NULL) {
        ALOGE("%s: argument cannot be NULL", __FUNCTION__);
 return BAD_VALUE;
 }

 CaptureResult &result = *(mResultQueue.begin());
    frame->mResultExtras = result.mResultExtras;
    frame->mMetadata.acquire(result.mMetadata);
    mResultQueue.erase(mResultQueue.begin());

 return OK;
}
