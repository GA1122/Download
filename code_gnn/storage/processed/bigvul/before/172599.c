status_t AudioFlinger::EffectChain::addEffect_l(const sp<EffectModule>& effect)
{
 effect_descriptor_t desc = effect->desc();
 uint32_t insertPref = desc.flags & EFFECT_FLAG_INSERT_MASK;

 Mutex::Autolock _l(mLock);
    effect->setChain(this);
    sp<ThreadBase> thread = mThread.promote();
 if (thread == 0) {
 return NO_INIT;
 }
    effect->setThread(thread);

 if ((desc.flags & EFFECT_FLAG_TYPE_MASK) == EFFECT_FLAG_TYPE_AUXILIARY) {
        mEffects.insertAt(effect, 0);

 size_t numSamples = thread->frameCount();
 int32_t *buffer = new int32_t[numSamples];
        memset(buffer, 0, numSamples * sizeof(int32_t));
        effect->setInBuffer((int16_t *)buffer);
        effect->setOutBuffer(mInBuffer);
 } else {

 size_t size = mEffects.size();
 size_t idx_insert = size;
 ssize_t idx_insert_first = -1;
 ssize_t idx_insert_last = -1;

 for (size_t i = 0; i < size; i++) {
 effect_descriptor_t d = mEffects[i]->desc();
 uint32_t iMode = d.flags & EFFECT_FLAG_TYPE_MASK;
 uint32_t iPref = d.flags & EFFECT_FLAG_INSERT_MASK;
 if (iMode == EFFECT_FLAG_TYPE_INSERT) {
 if (insertPref == EFFECT_FLAG_INSERT_EXCLUSIVE ||
                    iPref == EFFECT_FLAG_INSERT_EXCLUSIVE) {
                    ALOGW("addEffect_l() could not insert effect %s: exclusive conflict with %s",
                            desc.name, d.name);
 return INVALID_OPERATION;
 }
 if (idx_insert == size) {
                    idx_insert = i;
 }
 if (iPref == EFFECT_FLAG_INSERT_FIRST) {
                    idx_insert_first = i;
 }
 if (iPref == EFFECT_FLAG_INSERT_LAST &&
                    idx_insert_last == -1) {
                    idx_insert_last = i;
 }
 }
 }

 if (insertPref == EFFECT_FLAG_INSERT_LAST) {
 if (idx_insert_last != -1) {
                idx_insert = idx_insert_last;
 } else {
                idx_insert = size;
 }
 } else {
 if (idx_insert_first != -1) {
                idx_insert = idx_insert_first + 1;
 }
 }

        effect->setInBuffer(mInBuffer);

 if (idx_insert == size) {
 if (idx_insert != 0) {
                mEffects[idx_insert-1]->setOutBuffer(mInBuffer);
                mEffects[idx_insert-1]->configure();
 }
            effect->setOutBuffer(mOutBuffer);
 } else {
            effect->setOutBuffer(mInBuffer);
 }
        mEffects.insertAt(effect, idx_insert);

        ALOGV("addEffect_l() effect %p, added in chain %p at rank %d", effect.get(), this,
                idx_insert);
 }
    effect->configure();
 return NO_ERROR;
}
