status_t AudioFlinger::EffectModule::stop()
{
 Mutex::Autolock _l(mLock);
 return stop_l();
}
