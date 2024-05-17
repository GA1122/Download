void SoundTriggerHwService::onCallbackEvent(const sp<CallbackEvent>& event)
{
    ALOGV("onCallbackEvent");
    sp<Module> module;
 {
 AutoMutex lock(mServiceLock);
        module = event->mModule.promote();
 if (module == 0) {
 return;
 }
 }
    module->onCallbackEvent(event);
 {
 AutoMutex lock(mServiceLock);
        event->mMemory.clear();
 }
}
