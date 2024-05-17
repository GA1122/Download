bool WebContentsImpl::IsAudioMuted() const {
  if (base::FeatureList::IsEnabled(features::kAudioServiceAudioStreams)) {
    return audio_stream_factory_ && audio_stream_factory_->IsMuted();
  }
  return audio_muter_.get() && audio_muter_->is_muting();
}
