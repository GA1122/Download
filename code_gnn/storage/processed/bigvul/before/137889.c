void MediaControlMuteButtonElement::updateDisplayType() {
  setDisplayType((mediaElement().muted() || mediaElement().volume() == 0)
                     ? MediaUnMuteButton
                     : MediaMuteButton);
  updateOverflowString();
}
