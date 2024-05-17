void VirtualKeyboardController::OnMaximizeModeStarted() {
  if (!IsSmartVirtualKeyboardEnabled()) {
    SetKeyboardEnabled(true);
  } else {
    UpdateKeyboardEnabled();
  }
}
