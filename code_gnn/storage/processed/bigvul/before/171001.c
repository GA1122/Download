sp<AudioFlinger::EffectModule> AudioFlinger::EffectChain::getEffectIfEnabled(
 const effect_uuid_t *type)
{
    sp<EffectModule> effect = getEffectFromType_l(type);
 return effect != 0 && effect->isEnabled() ? effect : 0;
}
