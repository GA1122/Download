status_t BufferQueueConsumer::setConsumerUsageBits(uint32_t usage) {
    ATRACE_CALL();
    BQ_LOGV("setConsumerUsageBits: %#x", usage);
 Mutex::Autolock lock(mCore->mMutex);
    mCore->mConsumerUsageBits = usage;
 return NO_ERROR;
}
