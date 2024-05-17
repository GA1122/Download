bool WebContentsImpl::IsAudioMuted() const {
  return audio_muter_.get() && audio_muter_->is_muting();
}
