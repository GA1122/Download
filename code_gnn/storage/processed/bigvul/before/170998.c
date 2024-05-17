sp<AudioFlinger::EffectModule> AudioFlinger::EffectChain::getEffectFromDesc_l(
 effect_descriptor_t *descriptor)
{
 size_t size = mEffects.size();

 for (size_t i = 0; i < size; i++) {
 if (memcmp(&mEffects[i]->desc().uuid, &descriptor->uuid, sizeof(effect_uuid_t)) == 0) {
 return mEffects[i];
 }
 }
 return 0;
}
