status_t SoundTriggerHwService::attach(const sound_trigger_module_handle_t handle,
 const sp<ISoundTriggerClient>& client,
                        sp<ISoundTrigger>& moduleInterface)
{
    ALOGV("attach module %d", handle);
 if (!captureHotwordAllowed()) {
 return PERMISSION_DENIED;
 }

 AutoMutex lock(mServiceLock);
    moduleInterface.clear();
 if (client == 0) {
 return BAD_VALUE;
 }
 ssize_t index = mModules.indexOfKey(handle);
 if (index < 0) {
 return BAD_VALUE;
 }
    sp<Module> module = mModules.valueAt(index);

    module->setClient(client);
 IInterface::asBinder(client)->linkToDeath(module);
    moduleInterface = module;

    module->setCaptureState_l(mCaptureState);

 return NO_ERROR;
}
