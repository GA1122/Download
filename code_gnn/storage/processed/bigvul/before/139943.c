void HTMLMediaElement::cancelledRemotePlaybackRequest() {
  if (remotePlaybackClient())
    remotePlaybackClient()->promptCancelled();
}
