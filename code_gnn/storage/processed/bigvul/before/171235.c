SoundTriggerHwService::Module::Module(const sp<SoundTriggerHwService>& service,
                                      sound_trigger_hw_device* hwDevice,
                                      sound_trigger_module_descriptor descriptor,
 const sp<ISoundTriggerClient>& client)
 : mService(service), mHwDevice(hwDevice), mDescriptor(descriptor),
   mClient(client), mServiceState(SOUND_TRIGGER_STATE_NO_INIT)
{
}
