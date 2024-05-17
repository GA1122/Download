LRESULT HWNDMessageHandler::OnScrollMessage(UINT message,
                                            WPARAM w_param,
                                            LPARAM l_param) {
  MSG msg = { hwnd(), message, w_param, l_param, GetMessageTime() };
  ui::ScrollEvent event(msg);
  delegate_->HandleScrollEvent(event);
  return 0;
}
