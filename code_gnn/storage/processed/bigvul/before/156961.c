blink::WebMediaPlayer::ReadyState WebMediaPlayerMS::GetReadyState() const {
  DVLOG(1) << __func__ << ", state:" << ready_state_;
  DCHECK(thread_checker_.CalledOnValidThread());
  return ready_state_;
}
