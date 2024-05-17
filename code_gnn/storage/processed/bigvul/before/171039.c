void AudioFlinger::EffectChain::syncHalEffectsState()
{
 Mutex::Autolock _l(mLock);
 for (size_t i = 0; i < mEffects.size(); i++) {
 if (mEffects[i]->state() == EffectModule::ACTIVE ||
                mEffects[i]->state() == EffectModule::STOPPING) {
            mEffects[i]->addEffectToHal_l();
 }
 }
}
