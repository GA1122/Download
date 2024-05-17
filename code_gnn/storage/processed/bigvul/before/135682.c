void FrameSelection::SetUseSecureKeyboardEntry(bool enable) {
  if (enable)
    EnableSecureTextInput();
  else
    DisableSecureTextInput();
}
