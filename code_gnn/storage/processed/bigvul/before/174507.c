status_t BufferQueueConsumer::setDefaultBufferSize(uint32_t width,
 uint32_t height) {
    ATRACE_CALL();

 if (width == 0 || height == 0) {
        BQ_LOGV("setDefaultBufferSize: dimensions cannot be 0 (width=%u "
 "height=%u)", width, height);
 return BAD_VALUE;
 }

    BQ_LOGV("setDefaultBufferSize: width=%u height=%u", width, height);

 Mutex::Autolock lock(mCore->mMutex);
    mCore->mDefaultWidth = width;
    mCore->mDefaultHeight = height;
 return NO_ERROR;
}
