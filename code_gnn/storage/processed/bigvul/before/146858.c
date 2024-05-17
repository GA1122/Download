void Document::UpdateFocusAppearanceLater() {
  if (!update_focus_appearance_timer_.IsActive())
    update_focus_appearance_timer_.StartOneShot(0, BLINK_FROM_HERE);
}
