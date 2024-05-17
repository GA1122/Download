void HTMLMediaElement::RequestRemotePlaybackControl() {
  if (GetWebMediaPlayer())
    GetWebMediaPlayer()->RequestRemotePlaybackControl();
}
