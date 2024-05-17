LRESULT InputMethodIMM32::OnImeEndComposition(HWND window_handle,
                                              UINT message,
                                              WPARAM wparam,
                                              LPARAM lparam,
                                              BOOL* handled) {
  *handled = FALSE;

  composing_window_handle_ = NULL;

  if (!IsTextInputTypeNone() && GetTextInputClient()->HasCompositionText())
    GetTextInputClient()->ClearCompositionText();

  imm32_manager_.ResetComposition(window_handle);
  imm32_manager_.DestroyImeWindow(window_handle);
  return 0;
}
