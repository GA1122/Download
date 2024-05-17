bool WebMediaPlayerImpl::ShouldDisableVideoWhenHidden() const {
  if (!IsBackgroundVideoTrackOptimizationEnabled())
    return false;

  return HasAudio() && IsBackgroundOptimizationCandidate();
}
