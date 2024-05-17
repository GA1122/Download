LRESULT HWNDMessageHandler::OnKeyEvent(UINT message,
                                       WPARAM w_param,
                                       LPARAM l_param) {
  MSG msg = { hwnd(), message, w_param, l_param, GetMessageTime() };
  ui::KeyEvent key(msg, message == WM_CHAR);
  if (!delegate_->HandleUntranslatedKeyEvent(key))
    DispatchKeyEventPostIME(key);
  return 0;
}
