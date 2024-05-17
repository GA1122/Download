void AudioMixerAlsa::ApplyState() {
  DCHECK(MessageLoop::current() == thread_->message_loop());
  if (!alsa_mixer_)
    return;

  bool should_mute = false;
  double new_volume_db = 0;
  {
    base::AutoLock lock(lock_);
    should_mute = is_muted_;
    new_volume_db = should_mute ? min_volume_db_ : volume_db_;
    apply_is_pending_ = false;
  }

  if (pcm_element_) {
    SetElementVolume(master_element_, new_volume_db, 0.9999f);

    double pcm_volume_db = 0.0;
    double master_volume_db = 0.0;
    if (GetElementVolume(master_element_, &master_volume_db))
      pcm_volume_db = new_volume_db - master_volume_db;
    SetElementVolume(pcm_element_, pcm_volume_db, 0.5f);
  } else {
    SetElementVolume(master_element_, new_volume_db, 0.5f);
  }

  SetElementMuted(master_element_, should_mute);
}
