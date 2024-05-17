LRESULT InputMethodIMM32::OnImeComposition(HWND window_handle,
                                           UINT message,
                                           WPARAM wparam,
                                           LPARAM lparam,
                                           BOOL* handled) {
  *handled = TRUE;

  imm32_manager_.UpdateImeWindow(window_handle);

  ui::CompositionText composition;
  if (imm32_manager_.GetResult(window_handle, lparam, &composition.text)) {
    if (!IsTextInputTypeNone())
      GetTextInputClient()->InsertText(composition.text);
    imm32_manager_.ResetComposition(window_handle);
  }
  if (imm32_manager_.GetComposition(window_handle, lparam, &composition) &&
      !IsTextInputTypeNone())
    GetTextInputClient()->SetCompositionText(composition);

  return 0;
}
