void AudioMixerAlsa::SetVolumeDb(double volume_db) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  {
    base::AutoLock lock(lock_);
    if (isnan(volume_db)) {
      LOG(WARNING) << "Got request to set volume to NaN";
      volume_db = min_volume_db_;
    } else {
      volume_db = min(max(volume_db, min_volume_db_), max_volume_db_);
    }
  }

  prefs_->SetDouble(prefs::kAudioVolume, volume_db);

  base::AutoLock lock(lock_);
  volume_db_ = volume_db;
  if (!apply_is_pending_)
    thread_->message_loop()->PostTask(FROM_HERE,
        base::Bind(&AudioMixerAlsa::ApplyState, base::Unretained(this)));
}
