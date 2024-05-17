 void InputMethodIMM32::CancelComposition(const TextInputClient* client) {
  if (enabled_ && IsTextInputClientFocused(client))
    imm32_manager_.CancelIME(GetAttachedWindowHandle(client));
}
