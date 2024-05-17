void HWNDMessageHandler::Show(int show_state) {
  ShowWindow(hwnd(), show_state);
  if (show_state == SW_HIDE) {
    show_state = SW_SHOWNORMAL;
    ShowWindow(hwnd(), show_state);
  }

  if (show_state == SW_SHOWNORMAL || show_state == SW_SHOWMAXIMIZED)
    Activate();

  if (!delegate_->HandleInitialFocus())
    SetInitialFocus();
}
