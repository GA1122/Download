void AudioFlinger::EffectModule::setSuspended(bool suspended)
{
 Mutex::Autolock _l(mLock);
    mSuspended = suspended;
}
