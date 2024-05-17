void SoundTriggerHwService::sendServiceStateEvent_l(sound_trigger_service_state_t state,
 Module *module)
{
    sp<IMemory> eventMemory = prepareServiceStateEvent_l(state);
 if (eventMemory == 0) {
 return;
 }
    sp<Module> strongModule;
 for (size_t i = 0; i < mModules.size(); i++) {
 if (mModules.valueAt(i).get() == module) {
            strongModule = mModules.valueAt(i);
 break;
 }
 }
 if (strongModule == 0) {
 return;
 }
    sendCallbackEvent_l(new CallbackEvent(CallbackEvent::TYPE_SERVICE_STATE,
                                                 eventMemory, strongModule));
}
