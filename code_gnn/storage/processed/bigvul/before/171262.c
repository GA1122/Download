void SoundTriggerHwService::soundModelCallback(struct sound_trigger_model_event *event,
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

    service->sendSoundModelEvent(event, module);
}
