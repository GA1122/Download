void SoundTriggerHwService::detachModule(sp<Module> module)
{
    ALOGV("detachModule");
 AutoMutex lock(mServiceLock);
    module->clearClient();
}
