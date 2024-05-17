void HTMLMediaElement::ConnectedToRemoteDevice() {
  playing_remotely_ = true;
  if (RemotePlaybackClient())
    RemotePlaybackClient()->StateChanged(WebRemotePlaybackState::kConnecting);
}
