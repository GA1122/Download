bool HTMLMediaElement::IsFullscreen() const {
  return Fullscreen::IsFullscreenElement(*this);
}
