void HTMLMediaElement::clearMediaPlayer() {
  forgetResourceSpecificTracks();

  closeMediaSource();

  cancelDeferredLoad();

  {
    AudioSourceProviderClientLockScope scope(*this);
    clearMediaPlayerAndAudioSourceProviderClientWithoutLocking();
  }

  stopPeriodicTimers();
  m_loadTimer.stop();

  m_pendingActionFlags = 0;
  m_loadState = WaitingForSource;

  m_playingRemotely = false;
  remoteRouteAvailabilityChanged(WebRemotePlaybackAvailability::Unknown);

  if (layoutObject())
    layoutObject()->setShouldDoFullPaintInvalidation();
}
