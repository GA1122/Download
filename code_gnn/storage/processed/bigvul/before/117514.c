void AudioMixerAlsa::Connect() {
  DCHECK(MessageLoop::current() == thread_->message_loop());
  DCHECK(!alsa_mixer_);

  if (disconnected_event_.IsSignaled())
    return;

  if (!ConnectInternal()) {
    thread_->message_loop()->PostDelayedTask(FROM_HERE,
        base::Bind(&AudioMixerAlsa::Connect, base::Unretained(this)),
        kConnectionRetrySleepSec * 1000);
  }
}
