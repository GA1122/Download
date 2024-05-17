void FrameSelection::UpdateSecureKeyboardEntryIfActive() {
  if (!FrameIsFocusedAndActive())
    return;
  SetUseSecureKeyboardEntry(use_secure_keyboard_entry_when_active_);
}
