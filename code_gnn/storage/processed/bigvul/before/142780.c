void HTMLMediaElement::DisconnectedFromRemoteDevice() {
  playing_remotely_ = false;
  if (RemotePlaybackClient())
    RemotePlaybackClient()->StateChanged(WebRemotePlaybackState::kDisconnected);
}
