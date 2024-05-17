SoundTriggerHwService::~SoundTriggerHwService()
{
 if (mCallbackThread != 0) {
        mCallbackThread->exit();
 }
 for (size_t i = 0; i < mModules.size(); i++) {
        sound_trigger_hw_device_close(mModules.valueAt(i)->hwDevice());
 }
}
