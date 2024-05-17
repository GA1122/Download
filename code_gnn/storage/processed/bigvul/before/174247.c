status_t Camera3Device::prepare(int maxCount, int streamId) {
    ATRACE_CALL();
    ALOGV("%s: Camera %d: Preparing stream %d", __FUNCTION__, mId, streamId);
 Mutex::Autolock il(mInterfaceLock);
 Mutex::Autolock l(mLock);

    sp<Camera3StreamInterface> stream;
 ssize_t outputStreamIdx = mOutputStreams.indexOfKey(streamId);
 if (outputStreamIdx == NAME_NOT_FOUND) {
        CLOGE("Stream %d does not exist", streamId);
 return BAD_VALUE;
 }

    stream = mOutputStreams.editValueAt(outputStreamIdx);

 if (stream->isUnpreparable() || stream->hasOutstandingBuffers() ) {
        CLOGE("Stream %d has already been a request target", streamId);
 return BAD_VALUE;
 }

 if (mRequestThread->isStreamPending(stream)) {
        CLOGE("Stream %d is already a target in a pending request", streamId);
 return BAD_VALUE;
 }

 return mPreparerThread->prepare(maxCount, stream);
}
