sp<IMemory> SoundTriggerHwService::prepareRecognitionEvent_l(
 struct sound_trigger_recognition_event *event)
{
    sp<IMemory> eventMemory;

 switch (event->type) {
 case SOUND_MODEL_TYPE_KEYPHRASE:
        ALOGW_IF(event->data_size != 0 && event->data_offset !=
 sizeof(struct sound_trigger_phrase_recognition_event),
 "prepareRecognitionEvent_l(): invalid data offset %u for keyphrase event type",
                    event->data_offset);
        event->data_offset = sizeof(struct sound_trigger_phrase_recognition_event);
 break;
 case SOUND_MODEL_TYPE_GENERIC:
        ALOGW_IF(event->data_size != 0 && event->data_offset !=
 sizeof(struct sound_trigger_generic_recognition_event),
 "prepareRecognitionEvent_l(): invalid data offset %u for generic event type",
                    event->data_offset);
        event->data_offset = sizeof(struct sound_trigger_generic_recognition_event);
 break;
 case SOUND_MODEL_TYPE_UNKNOWN:
        ALOGW_IF(event->data_size != 0 && event->data_offset !=
 sizeof(struct sound_trigger_recognition_event),
 "prepareRecognitionEvent_l(): invalid data offset %u for unknown event type",
                    event->data_offset);
        event->data_offset = sizeof(struct sound_trigger_recognition_event);
 break;
 default:
 return eventMemory;
 }

 size_t size = event->data_offset + event->data_size;
    eventMemory = mMemoryDealer->allocate(size);
 if (eventMemory == 0 || eventMemory->pointer() == NULL) {
        eventMemory.clear();
 return eventMemory;
 }
    memcpy(eventMemory->pointer(), event, size);

 return eventMemory;
}
