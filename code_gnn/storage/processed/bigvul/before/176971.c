void InputDispatcher::notifyConfigurationChanged(const NotifyConfigurationChangedArgs* args) {
#if DEBUG_INBOUND_EVENT_DETAILS
    ALOGD("notifyConfigurationChanged - eventTime=%lld", args->eventTime);
#endif

 bool needWake;
 {  
 AutoMutex _l(mLock);

 ConfigurationChangedEntry* newEntry = new ConfigurationChangedEntry(args->eventTime);
        needWake = enqueueInboundEventLocked(newEntry);
 }  

 if (needWake) {
        mLooper->wake();
 }
}
