void WebRtcAudioRenderer::SetVolume(float volume) {
  base::AutoLock auto_lock(lock_);
  if (state_ == UNINITIALIZED)
    return;

  sink_->SetVolume(volume);
}
