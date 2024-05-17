bool HTMLMediaElement::hasAudio() const {
  return webMediaPlayer() && webMediaPlayer()->hasAudio();
}
