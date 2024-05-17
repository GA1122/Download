void AudioFlinger::EffectHandle::dumpToBuffer(char* buffer, size_t size)
{
 bool locked = mCblk != NULL && AudioFlinger::dumpTryLock(mCblk->lock);

    snprintf(buffer, size, "\t\t\t%5d    %5d  %3s    %3s  %5u  %5u\n",
 (mClient == 0) ? getpid_cached : mClient->pid(),
            mPriority,
            mHasControl ? "yes" : "no",
            locked ? "yes" : "no",
            mCblk ? mCblk->clientIndex : 0,
            mCblk ? mCblk->serverIndex : 0
 );

 if (locked) {
        mCblk->lock.unlock();
 }
}
