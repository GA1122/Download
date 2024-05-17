bool WebMediaPlayerImpl::ShouldDisableVideoWhenHidden() const {
  if (!IsBackgroundVideoTrackOptimizationEnabled(load_type_))
    return false;

  return HasAudio() && IsBackgroundOptimizationCandidate();
}
