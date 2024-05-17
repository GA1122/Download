void AudioFlinger::EffectChain::setAudioSource_l(audio_source_t source)
{
 size_t size = mEffects.size();
 for (size_t i = 0; i < size; i++) {
        mEffects[i]->setAudioSource(source);
 }
}
