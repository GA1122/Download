bool HTMLMediaElement::HasRemoteRoutes() const {
  return RemotePlaybackClient() &&
         RemotePlaybackClient()->RemotePlaybackAvailable();
}
