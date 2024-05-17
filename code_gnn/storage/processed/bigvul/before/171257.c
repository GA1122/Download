void SoundTriggerHwService::sendRecognitionEvent(struct sound_trigger_recognition_event *event,
 Module *module)
 {
 AutoMutex lock(mServiceLock);
 if (module == NULL) {
 return;
 }
     sp<IMemory> eventMemory = prepareRecognitionEvent_l(event);
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

     sendCallbackEvent_l(new CallbackEvent(CallbackEvent::TYPE_RECOGNITION,
                                                  eventMemory, strongModule));
}
