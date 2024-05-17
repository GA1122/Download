sp<IMemory> SoundTriggerHwService::prepareServiceStateEvent_l(sound_trigger_service_state_t state)
{
    sp<IMemory> eventMemory;

 size_t size = sizeof(sound_trigger_service_state_t);
    eventMemory = mMemoryDealer->allocate(size);
 if (eventMemory == 0 || eventMemory->pointer() == NULL) {
        eventMemory.clear();
 return eventMemory;
 }
 *((sound_trigger_service_state_t *)eventMemory->pointer()) = state;
 return eventMemory;
}
