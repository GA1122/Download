bool AudioMixerAlsa::IsInitialized() {
  base::AutoLock lock(lock_);
  return alsa_mixer_ != NULL;
}
