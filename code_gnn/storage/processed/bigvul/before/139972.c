void HTMLMediaElement::disconnectedFromRemoteDevice() {
  m_playingRemotely = false;
  if (remotePlaybackClient())
    remotePlaybackClient()->stateChanged(WebRemotePlaybackState::Disconnected);

  if (mediaControls())
    mediaControls()->onRemotePlaybackDisconnected();
}
