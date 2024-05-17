bool AudioFlinger::EffectChain::isNonOffloadableEnabled()
{
 Mutex::Autolock _l(mLock);
 size_t size = mEffects.size();
 for (size_t i = 0; i < size; i++) {
 if (mEffects[i]->isEnabled() && !mEffects[i]->isOffloadable()) {
 return true;
 }
 }
 return false;
}
