bool AudioFlinger::EffectModule::isOffloaded() const
{
 Mutex::Autolock _l(mLock);
 return mOffloaded;
}
