void WebRtcAudioRenderer::Stop() {
  base::AutoLock auto_lock(lock_);
  if (state_ == UNINITIALIZED)
    return;

  source_->RemoveRenderer(this);
  source_ = NULL;
  sink_->Stop();
  state_ = UNINITIALIZED;
}
