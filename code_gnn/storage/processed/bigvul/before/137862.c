MediaControlCastButtonElement::getOverflowStringName() {
  if (mediaElement().isPlayingRemotely())
    return WebLocalizedString::OverflowMenuStopCast;
  return WebLocalizedString::OverflowMenuCast;
}
