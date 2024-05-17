status_t AudioFlinger::EffectModule::start()
{
 Mutex::Autolock _l(mLock);
 return start_l();
}
