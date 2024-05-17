void WebRtcAudioRenderer::Play() {
  base::AutoLock auto_lock(lock_);
  if (state_ == UNINITIALIZED)
    return;

  state_ = PLAYING;
}
