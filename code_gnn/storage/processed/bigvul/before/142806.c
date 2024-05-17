bool HTMLMediaElement::HasAudio() const {
  return GetWebMediaPlayer() && GetWebMediaPlayer()->HasAudio();
}
