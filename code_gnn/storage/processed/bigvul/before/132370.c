void VirtualKeyboardController::SetKeyboardEnabled(bool enabled) {
  keyboard::SetTouchKeyboardEnabled(enabled);
  if (enabled) {
    Shell::GetInstance()->CreateKeyboard();
  } else {
    if (!keyboard::IsKeyboardEnabled())
      Shell::GetInstance()->DeactivateKeyboard();
  }
}
