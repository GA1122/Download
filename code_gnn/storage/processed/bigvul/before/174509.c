status_t BufferQueueConsumer::setMaxAcquiredBufferCount(
 int maxAcquiredBuffers) {
    ATRACE_CALL();

 if (maxAcquiredBuffers < 1 ||
            maxAcquiredBuffers > BufferQueueCore::MAX_MAX_ACQUIRED_BUFFERS) {
        BQ_LOGE("setMaxAcquiredBufferCount: invalid count %d",
                maxAcquiredBuffers);
 return BAD_VALUE;
 }

 Mutex::Autolock lock(mCore->mMutex);

 if (mCore->mConnectedApi != BufferQueueCore::NO_CONNECTED_API) {
        BQ_LOGE("setMaxAcquiredBufferCount: producer is already connected");
 return INVALID_OPERATION;
 }

    BQ_LOGV("setMaxAcquiredBufferCount: %d", maxAcquiredBuffers);
    mCore->mMaxAcquiredBufferCount = maxAcquiredBuffers;
 return NO_ERROR;
}
