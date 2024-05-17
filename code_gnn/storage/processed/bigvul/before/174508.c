status_t BufferQueueConsumer::setDefaultMaxBufferCount(int bufferCount) {
    ATRACE_CALL();
 Mutex::Autolock lock(mCore->mMutex);
 return mCore->setDefaultMaxBufferCountLocked(bufferCount);
}
