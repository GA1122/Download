status_t BufferQueueConsumer::connect(
 const sp<IConsumerListener>& consumerListener, bool controlledByApp) {
    ATRACE_CALL();

 if (consumerListener == NULL) {
        BQ_LOGE("connect(C): consumerListener may not be NULL");
 return BAD_VALUE;
 }

    BQ_LOGV("connect(C): controlledByApp=%s",
            controlledByApp ? "true" : "false");

 Mutex::Autolock lock(mCore->mMutex);

 if (mCore->mIsAbandoned) {
        BQ_LOGE("connect(C): BufferQueue has been abandoned");
 return NO_INIT;
 }

    mCore->mConsumerListener = consumerListener;
    mCore->mConsumerControlledByApp = controlledByApp;

 return NO_ERROR;
}
