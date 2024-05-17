MediaControlFullscreenButtonElement::getOverflowStringName() {
  if (mediaElement().isFullscreen())
    return WebLocalizedString::OverflowMenuExitFullscreen;
  return WebLocalizedString::OverflowMenuEnterFullscreen;
}
