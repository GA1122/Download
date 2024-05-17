void SoundTriggerHwService::recognitionCallback(struct sound_trigger_recognition_event *event,
 void *cookie)
{
 Module *module = (Module *)cookie;
 if (module == NULL) {
 return;
 }
    sp<SoundTriggerHwService> service = module->service().promote();
 if (service == 0) {
 return;
 }

    service->sendRecognitionEvent(event, module);
}
