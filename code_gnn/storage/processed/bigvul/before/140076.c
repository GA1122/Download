void HTMLMediaElement::resetMediaPlayerAndMediaSource() {
  closeMediaSource();

  {
    AudioSourceProviderClientLockScope scope(*this);
    clearMediaPlayerAndAudioSourceProviderClientWithoutLocking();
  }

  m_playingRemotely = false;
  remoteRouteAvailabilityChanged(WebRemotePlaybackAvailability::Unknown);

  if (m_audioSourceNode)
    getAudioSourceProvider().setClient(m_audioSourceNode);
}
