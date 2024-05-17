sp<AudioFlinger::EffectModule> AudioFlinger::EffectChain::getEffectFromType_l(
 const effect_uuid_t *type)
{
 size_t size = mEffects.size();

 for (size_t i = 0; i < size; i++) {
 if (memcmp(&mEffects[i]->desc().type, type, sizeof(effect_uuid_t)) == 0) {
 return mEffects[i];
 }
 }
 return 0;
}
