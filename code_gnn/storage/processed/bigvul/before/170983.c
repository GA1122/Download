void AudioFlinger::EffectChain::clearInputBuffer()
{
 Mutex::Autolock _l(mLock);
    sp<ThreadBase> thread = mThread.promote();
 if (thread == 0) {
        ALOGW("clearInputBuffer(): cannot promote mixer thread");
 return;
 }
    clearInputBuffer_l(thread);
}
