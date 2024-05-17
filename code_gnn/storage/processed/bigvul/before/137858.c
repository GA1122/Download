MediaControlPlayButtonElement::getOverflowStringName() {
  if (mediaElement().paused())
    return WebLocalizedString::OverflowMenuPlay;
  return WebLocalizedString::OverflowMenuPause;
}
