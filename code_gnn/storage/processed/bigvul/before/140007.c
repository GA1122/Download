bool HTMLMediaElement::isFullscreen() const {
  return Fullscreen::isCurrentFullScreenElement(*this);
}
