void AudioFlinger::EffectChain::setDevice_l(audio_devices_t device)
{
 size_t size = mEffects.size();
 for (size_t i = 0; i < size; i++) {
        mEffects[i]->setDevice(device);
 }
}
