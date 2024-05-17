bool MediaControlVolumeSliderElement::willRespondToMouseClickEvents() {
  if (!isConnected() || !document().isActive())
    return false;

  return MediaControlInputElement::willRespondToMouseClickEvents();
}
