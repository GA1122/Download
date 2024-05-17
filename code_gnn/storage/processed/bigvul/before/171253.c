sp<IMemory> SoundTriggerHwService::prepareSoundModelEvent_l(struct sound_trigger_model_event *event)
{
    sp<IMemory> eventMemory;

 size_t size = event->data_offset + event->data_size;
    eventMemory = mMemoryDealer->allocate(size);
 if (eventMemory == 0 || eventMemory->pointer() == NULL) {
        eventMemory.clear();
 return eventMemory;
 }
    memcpy(eventMemory->pointer(), event, size);

 return eventMemory;
}
