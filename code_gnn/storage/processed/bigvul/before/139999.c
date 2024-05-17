bool HTMLMediaElement::hasVideo() const {
  return webMediaPlayer() && webMediaPlayer()->hasVideo();
}
