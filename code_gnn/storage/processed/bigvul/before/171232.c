SoundTriggerHwService::CallbackEvent::CallbackEvent(event_type type, sp<IMemory> memory,
                                                    wp<Module> module)
 : mType(type), mMemory(memory), mModule(module)
{
}
