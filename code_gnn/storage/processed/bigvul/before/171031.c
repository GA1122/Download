void AudioFlinger::EffectChain::setThread(const sp<ThreadBase>& thread)
{
 Mutex::Autolock _l(mLock);
    mThread = thread;
 for (size_t i = 0; i < mEffects.size(); i++) {
        mEffects[i]->setThread(thread);
 }
}
