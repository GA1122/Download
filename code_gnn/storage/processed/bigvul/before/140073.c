void HTMLMediaElement::requestRemotePlaybackStop() {
  if (webMediaPlayer())
    webMediaPlayer()->requestRemotePlaybackStop();
}
