void HTMLMediaElement::FlingingStarted() {
  if (GetWebMediaPlayer())
    GetWebMediaPlayer()->FlingingStarted();
}
