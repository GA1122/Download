status_t AudioFlinger::EffectModule::setEnabled(bool enabled)
{
 Mutex::Autolock _l(mLock);
 return setEnabled_l(enabled);
}
