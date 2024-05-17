    isGestureNeededForPlaybackIfCrossOriginExperimentEnabled() const {
  if (!m_lockedPendingUserGestureIfCrossOriginExperimentEnabled)
    return false;

  return isGestureNeededForPlaybackIfPendingUserGestureIsLocked();
}
