status_t SoundTriggerHwService::listModules(struct sound_trigger_module_descriptor *modules,
 uint32_t *numModules)
{
    ALOGV("listModules");
 if (!captureHotwordAllowed()) {
 return PERMISSION_DENIED;
 }

 AutoMutex lock(mServiceLock);
 if (numModules == NULL || (*numModules != 0 && modules == NULL)) {
 return BAD_VALUE;
 }
 size_t maxModules = *numModules;
 *numModules = mModules.size();
 for (size_t i = 0; i < mModules.size() && i < maxModules; i++) {
        modules[i] = mModules.valueAt(i)->descriptor();
 }
 return NO_ERROR;
}
