void InputMethodIMM32::OnTextInputTypeChanged(const TextInputClient* client) {
  if (IsTextInputClientFocused(client) && IsWindowFocused(client)) {
    imm32_manager_.CancelIME(GetAttachedWindowHandle(client));
    UpdateIMEState();
  }
  InputMethodWin::OnTextInputTypeChanged(client);
 }
