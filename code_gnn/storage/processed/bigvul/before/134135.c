LRESULT InputMethodIMM32::OnImeStartComposition(HWND window_handle,
                                                UINT message,
                                                WPARAM wparam,
                                                LPARAM lparam,
                                                BOOL* handled) {
  *handled = TRUE;

  composing_window_handle_ = window_handle;
  imm32_manager_.CreateImeWindow(window_handle);
  imm32_manager_.ResetComposition(window_handle);
  return 0;
}
