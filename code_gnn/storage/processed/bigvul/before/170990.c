size_t AudioFlinger::EffectModule::disconnect(EffectHandle *handle, bool unpinIfLast)
{
    ALOGV("disconnect() %p handle %p", this, handle);
    sp<EffectModule> keep(this);
 {
 if (removeHandle(handle) == 0) {
 if (!isPinned() || unpinIfLast) {
                sp<ThreadBase> thread = mThread.promote();
 if (thread != 0) {
 Mutex::Autolock _l(thread->mLock);
                    thread->removeEffect_l(this);
 }
                sp<AudioFlinger> af = mAudioFlinger.promote();
 if (af != 0) {
                    af->updateOrphanEffectChains(this);
 }
 AudioSystem::unregisterEffect(mId);
 }
 }
 }
 return mHandles.size();
}
