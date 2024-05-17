void InputDispatcher::notifyDeviceReset(const NotifyDeviceResetArgs* args) {
#if DEBUG_INBOUND_EVENT_DETAILS
    ALOGD("notifyDeviceReset - eventTime=%lld, deviceId=%d",
            args->eventTime, args->deviceId);
#endif

 bool needWake;
 {  
 AutoMutex _l(mLock);

 DeviceResetEntry* newEntry = new DeviceResetEntry(args->eventTime, args->deviceId);
        needWake = enqueueInboundEventLocked(newEntry);
 }  

 if (needWake) {
        mLooper->wake();
 }
}
