status_t BufferQueueConsumer::setTransformHint(uint32_t hint) {
    ATRACE_CALL();
    BQ_LOGV("setTransformHint: %#x", hint);
 Mutex::Autolock lock(mCore->mMutex);
    mCore->mTransformHint = hint;
 return NO_ERROR;
}
