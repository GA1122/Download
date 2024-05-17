status_t Camera3Device::triggerCancelAutofocus(uint32_t id) {
    ATRACE_CALL();
 Mutex::Autolock il(mInterfaceLock);

    ALOGV("%s: Triggering cancel autofocus, id %d", __FUNCTION__, id);
 RequestTrigger trigger[] = {
 {
            ANDROID_CONTROL_AF_TRIGGER,
            ANDROID_CONTROL_AF_TRIGGER_CANCEL
 },
 {
            ANDROID_CONTROL_AF_TRIGGER_ID,
 static_cast<int32_t>(id)
 }
 };

 return mRequestThread->queueTrigger(trigger,
 sizeof(trigger)/sizeof(trigger[0]));
}
