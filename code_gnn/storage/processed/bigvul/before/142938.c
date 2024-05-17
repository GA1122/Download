void HTMLMediaElement::UpdateTextTrackDisplay() {
  BLINK_MEDIA_LOG << "updateTextTrackDisplay(" << (void*)this << ")";

  EnsureTextTrackContainer().UpdateDisplay(
      *this, TextTrackContainer::kDidNotStartExposingControls);
}
