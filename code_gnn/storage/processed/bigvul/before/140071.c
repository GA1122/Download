void HTMLMediaElement::requestRemotePlayback() {
  if (webMediaPlayer())
    webMediaPlayer()->requestRemotePlayback();
}
