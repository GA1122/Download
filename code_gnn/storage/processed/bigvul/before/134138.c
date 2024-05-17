void InputMethodIMM32::OnWillChangeFocusedClient(
    TextInputClient* focused_before,
    TextInputClient* focused) {
  if (IsWindowFocused(focused_before)) {
    ConfirmCompositionText();
  }
}
