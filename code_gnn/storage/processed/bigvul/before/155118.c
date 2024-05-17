void OmniboxViewViews::HideImeIfNeeded() {
  if (auto* input_method = GetInputMethod()) {
    if (auto* keyboard = input_method->GetInputMethodKeyboardController())
      keyboard->DismissVirtualKeyboard();
  }
}
