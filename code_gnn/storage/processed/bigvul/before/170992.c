void AudioFlinger::EffectHandle::disconnect(bool unpinIfLast)
{
    ALOGV("disconnect(%s)", unpinIfLast ? "true" : "false");
 if (mEffect == 0) {
 return;
 }
 if ((mEffect->disconnect(this, unpinIfLast) == 0) && mEnabled) {
        sp<ThreadBase> thread = mEffect->thread().promote();
 if (thread != 0) {
            thread->checkSuspendOnEffectEnabled(mEffect, false, mEffect->sessionId());
 }
 }

    mEffect.clear();
 if (mClient != 0) {
 if (mCblk != NULL) {
            mCblk->~effect_param_cblk_t();  
 }
        mCblkMemory.clear();  
 Mutex::Autolock _l(mClient->audioFlinger()->mClientLock);
        mClient.clear();
 }
}
