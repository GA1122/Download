void InputMethodIMM32::ConfirmCompositionText() {
  if (composing_window_handle_)
    imm32_manager_.CleanupComposition(composing_window_handle_);

  if (!IsTextInputTypeNone()) {
    if (GetTextInputClient()->HasCompositionText())
      GetTextInputClient()->ConfirmCompositionText();
  }
}
