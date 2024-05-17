void AudioRendererAlgorithm::SetPlaybackRate(float new_rate) {
  DCHECK_GE(new_rate, 0.0);
  playback_rate_ = new_rate;
  muted_ =
      playback_rate_ < kMinPlaybackRate || playback_rate_ > kMaxPlaybackRate;

  ResetWindow();
}
