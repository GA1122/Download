double AudioMixerAlsa::GetVolumeDb() {
  base::AutoLock lock(lock_);
  return volume_db_;
}
