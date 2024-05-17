void HTMLMediaElement::RequestRemotePlaybackStop() {
  if (GetWebMediaPlayer())
    GetWebMediaPlayer()->RequestRemotePlaybackStop();
}
