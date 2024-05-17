void MediaControlToggleClosedCaptionsButtonElement::updateDisplayType() {
  bool captionsVisible = mediaElement().textTracksVisible();
  setDisplayType(captionsVisible ? MediaHideClosedCaptionsButton
                                 : MediaShowClosedCaptionsButton);
}
