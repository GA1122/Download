void AudioFlinger::EffectChain::setMode_l(audio_mode_t mode)
{
 size_t size = mEffects.size();
 for (size_t i = 0; i < size; i++) {
        mEffects[i]->setMode(mode);
 }
}
