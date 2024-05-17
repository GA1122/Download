void HTMLMediaElement::connectedToRemoteDevice() {
  m_playingRemotely = true;
  if (remotePlaybackClient())
    remotePlaybackClient()->stateChanged(WebRemotePlaybackState::Connecting);

  if (mediaControls())
    mediaControls()->onRemotePlaybackConnecting();
}
