void MediaControlOverlayPlayButtonElement::updateDisplayType() {
  setIsWanted(mediaElement().shouldShowControls() && mediaElement().paused());
}
