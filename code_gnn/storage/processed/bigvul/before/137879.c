void MediaControlCastButtonElement::setIsPlayingRemotely(
    bool isPlayingRemotely) {
  if (isPlayingRemotely) {
    if (m_isOverlayButton) {
      setDisplayType(MediaOverlayCastOnButton);
    } else {
      setDisplayType(MediaCastOnButton);
    }
  } else {
    if (m_isOverlayButton) {
      setDisplayType(MediaOverlayCastOffButton);
    } else {
      setDisplayType(MediaCastOffButton);
    }
  }
  updateOverflowString();
}
