LRESULT InputMethodIMM32::OnImeSetContext(HWND window_handle,
                                          UINT message,
                                          WPARAM wparam,
                                          LPARAM lparam,
                                          BOOL* handled) {
  if (!!wparam)
    imm32_manager_.CreateImeWindow(window_handle);

  OnInputMethodChanged();
  return imm32_manager_.SetImeWindowStyle(
      window_handle, message, wparam, lparam, handled);
}
