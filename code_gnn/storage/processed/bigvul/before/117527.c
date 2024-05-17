void AudioMixerAlsa::SetMuted(bool muted) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  prefs_->SetInteger(prefs::kAudioMute, muted ? kPrefMuteOn : kPrefMuteOff);
  base::AutoLock lock(lock_);
  is_muted_ = muted;
  if (!apply_is_pending_)
    thread_->message_loop()->PostTask(FROM_HERE,
        base::Bind(&AudioMixerAlsa::ApplyState, base::Unretained(this)));
}
