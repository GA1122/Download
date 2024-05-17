status_t SoundTriggerHwService::Module::unloadSoundModel(sound_model_handle_t handle)
{
    ALOGV("unloadSoundModel() model handle %d", handle);
 if (!captureHotwordAllowed()) {
 return PERMISSION_DENIED;
 }

 AutoMutex lock(mLock);
 return unloadSoundModel_l(handle);
}
