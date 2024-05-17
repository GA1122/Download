void AddScrollStylesToWindow(HWND window) {
  if (::IsWindow(window)) {
    long current_style = ::GetWindowLong(window, GWL_STYLE);
    ::SetWindowLong(window, GWL_STYLE,
                    current_style | WS_VSCROLL | WS_HSCROLL);
  }
}
