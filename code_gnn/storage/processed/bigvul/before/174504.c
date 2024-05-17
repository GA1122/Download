void BufferQueueConsumer::setConsumerName(const String8& name) {
    ATRACE_CALL();
    BQ_LOGV("setConsumerName: '%s'", name.string());
 Mutex::Autolock lock(mCore->mMutex);
    mCore->mConsumerName = name;
    mConsumerName = name;
}
