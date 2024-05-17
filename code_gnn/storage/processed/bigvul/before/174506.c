status_t BufferQueueConsumer::setDefaultBufferFormat(uint32_t defaultFormat) {
    ATRACE_CALL();
    BQ_LOGV("setDefaultBufferFormat: %u", defaultFormat);
 Mutex::Autolock lock(mCore->mMutex);
    mCore->mDefaultBufferFormat = defaultFormat;
 return NO_ERROR;
}
