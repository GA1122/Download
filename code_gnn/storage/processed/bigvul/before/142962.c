double HTMLMediaElement::playbackRate() const {
  if (GetLoadType() == WebMediaPlayer::kLoadTypeMediaStream)
    return 1.0;
  return playback_rate_;
}
