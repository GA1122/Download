void AudioFlinger::EffectChain::setEffectSuspendedAll_l(bool suspend)
{
    sp<SuspendedEffectDesc> desc;

 ssize_t index = mSuspendedEffects.indexOfKey((int)kKeyForSuspendAll);
 if (suspend) {
 if (index >= 0) {
            desc = mSuspendedEffects.valueAt(index);
 } else {
            desc = new SuspendedEffectDesc();
            mSuspendedEffects.add((int)kKeyForSuspendAll, desc);
            ALOGV("setEffectSuspendedAll_l() add entry for 0");
 }
 if (desc->mRefCount++ == 0) {
 Vector< sp<EffectModule> > effects;
            getSuspendEligibleEffects(effects);
 for (size_t i = 0; i < effects.size(); i++) {
                setEffectSuspended_l(&effects[i]->desc().type, true);
 }
 }
 } else {
 if (index < 0) {
 return;
 }
        desc = mSuspendedEffects.valueAt(index);
 if (desc->mRefCount <= 0) {
            ALOGW("setEffectSuspendedAll_l() restore refcount should not be 0 %d", desc->mRefCount);
            desc->mRefCount = 1;
 }
 if (--desc->mRefCount == 0) {
 Vector<const effect_uuid_t *> types;
 for (size_t i = 0; i < mSuspendedEffects.size(); i++) {
 if (mSuspendedEffects.keyAt(i) == (int)kKeyForSuspendAll) {
 continue;
 }
                types.add(&mSuspendedEffects.valueAt(i)->mType);
 }
 for (size_t i = 0; i < types.size(); i++) {
                setEffectSuspended_l(types[i], false);
 }
            ALOGV("setEffectSuspendedAll_l() remove entry for %08x",
                    mSuspendedEffects.keyAt(index));
            mSuspendedEffects.removeItem((int)kKeyForSuspendAll);
 }
 }
}
