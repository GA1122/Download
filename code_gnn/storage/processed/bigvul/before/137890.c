void MediaControlPlayButtonElement::updateDisplayType() {
  setDisplayType(mediaElement().paused() ? MediaPlayButton : MediaPauseButton);
  updateOverflowString();
}
