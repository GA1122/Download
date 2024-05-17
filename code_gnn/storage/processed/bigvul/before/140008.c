bool HTMLMediaElement::isGestureNeededForPlayback() const {
  if (!m_lockedPendingUserGesture)
    return false;

  return isGestureNeededForPlaybackIfPendingUserGestureIsLocked();
}
