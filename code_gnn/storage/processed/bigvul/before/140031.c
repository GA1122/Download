void HTMLMediaElement::mediaControlsDidBecomeVisible() {
  BLINK_MEDIA_LOG << "mediaControlsDidBecomeVisible(" << (void*)this << ")";

  if (isHTMLVideoElement() && textTracksVisible())
    ensureTextTrackContainer().updateDisplay(
        *this, TextTrackContainer::DidStartExposingControls);
}
