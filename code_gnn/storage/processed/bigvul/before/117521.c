void AudioMixerAlsa::Init() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  prefs_ = g_browser_process->local_state();
  volume_db_ = prefs_->GetDouble(prefs::kAudioVolume);
  is_muted_ = prefs_->GetInteger(prefs::kAudioMute);

  DCHECK(!thread_.get()) << "Init() called twice";
  thread_.reset(new base::Thread("AudioMixerAlsa"));
  CHECK(thread_->Start());
  thread_->message_loop()->PostTask(
      FROM_HERE, base::Bind(&AudioMixerAlsa::Connect, base::Unretained(this)));
}
