void FrameSelection::SetUseSecureKeyboardEntryWhenActive(
    bool uses_secure_keyboard) {
  if (use_secure_keyboard_entry_when_active_ == uses_secure_keyboard)
    return;
  use_secure_keyboard_entry_when_active_ = uses_secure_keyboard;
  UpdateSecureKeyboardEntryIfActive();
}
