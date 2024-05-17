double HTMLMediaElement::defaultPlaybackRate() const {
  if (GetLoadType() == WebMediaPlayer::kLoadTypeMediaStream)
    return 1.0;
  return default_playback_rate_;
}
