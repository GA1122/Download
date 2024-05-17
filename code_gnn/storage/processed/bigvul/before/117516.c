void AudioMixerAlsa::Disconnect() {
  DCHECK(MessageLoop::current() == thread_->message_loop());
  if (alsa_mixer_) {
    snd_mixer_close(alsa_mixer_);
    alsa_mixer_ = NULL;
  }
  disconnected_event_.Signal();
}
