bool HTMLMediaElement::hasRemoteRoutes() const {
  return remotePlaybackClient() &&
         remotePlaybackClient()->remotePlaybackAvailable();
}
