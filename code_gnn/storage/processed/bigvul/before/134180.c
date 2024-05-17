LRESULT InputMethodWin::OnChar(HWND window_handle,
                               UINT message,
                               WPARAM wparam,
                               LPARAM lparam,
                               BOOL* handled) {
  *handled = TRUE;

  if (GetTextInputClient()) {
    GetTextInputClient()->InsertChar(static_cast<char16>(wparam),
                                     ui::GetModifiersFromKeyState());
  }

  if (message == WM_SYSCHAR && wparam == VK_SPACE)
    gfx::ShowSystemMenu(window_handle);

  return 0;
}
