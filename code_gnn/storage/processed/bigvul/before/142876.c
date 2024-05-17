void HTMLMediaElement::RequestRemotePlayback() {
  if (GetWebMediaPlayer())
    GetWebMediaPlayer()->RequestRemotePlayback();
}
