double HTMLMediaElement::CurrentPlaybackPosition() const {
  if (ready_state_ == kHaveNothing)
    return 0;

  if (GetWebMediaPlayer())
    return GetWebMediaPlayer()->CurrentTime();

  if (ready_state_ >= kHaveMetadata) {
    BLINK_MEDIA_LOG
        << __func__ << " readyState = " << ready_state_
        << " but no webMediaPlayer to provide currentPlaybackPosition";
  }

  return 0;
}
