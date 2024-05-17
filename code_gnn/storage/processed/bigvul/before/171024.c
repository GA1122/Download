void AudioFlinger::EffectChain::setEffectSuspended_l(
 const effect_uuid_t *type, bool suspend)
{
    sp<SuspendedEffectDesc> desc;
 ssize_t index = mSuspendedEffects.indexOfKey(type->timeLow);
 if (suspend) {
 if (index >= 0) {
            desc = mSuspendedEffects.valueAt(index);
 } else {
            desc = new SuspendedEffectDesc();
            desc->mType = *type;
            mSuspendedEffects.add(type->timeLow, desc);
            ALOGV("setEffectSuspended_l() add entry for %08x", type->timeLow);
 }
 if (desc->mRefCount++ == 0) {
            sp<EffectModule> effect = getEffectIfEnabled(type);
 if (effect != 0) {
                desc->mEffect = effect;
                effect->setSuspended(true);
                effect->setEnabled(false);
 }
 }
 } else {
 if (index < 0) {
 return;
 }
        desc = mSuspendedEffects.valueAt(index);
 if (desc->mRefCount <= 0) {
            ALOGW("setEffectSuspended_l() restore refcount should not be 0 %d", desc->mRefCount);
            desc->mRefCount = 1;
 }
 if (--desc->mRefCount == 0) {
            ALOGV("setEffectSuspended_l() remove entry for %08x", mSuspendedEffects.keyAt(index));
 if (desc->mEffect != 0) {
                sp<EffectModule> effect = desc->mEffect.promote();
 if (effect != 0) {
                    effect->setSuspended(false);
                    effect->lock();
 EffectHandle *handle = effect->controlHandle_l();
 if (handle != NULL && !handle->destroyed_l()) {
                        effect->setEnabled_l(handle->enabled());
 }
                    effect->unlock();
 }
                desc->mEffect.clear();
 }
            mSuspendedEffects.removeItemsAt(index);
 }
 }
}
