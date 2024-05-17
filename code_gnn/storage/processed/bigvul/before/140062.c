void HTMLMediaElement::remotePlaybackStarted() {
  if (remotePlaybackClient())
    remotePlaybackClient()->stateChanged(WebRemotePlaybackState::Connected);
}
