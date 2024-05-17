status_t InputDispatcher::unregisterInputChannel(const sp<InputChannel>& inputChannel) {
#if DEBUG_REGISTRATION
    ALOGD("channel '%s' ~ unregisterInputChannel", inputChannel->getName().string());
#endif

 {  
 AutoMutex _l(mLock);

 status_t status = unregisterInputChannelLocked(inputChannel, false  );
 if (status) {
 return status;
 }
 }  

    mLooper->wake();
 return OK;
}
