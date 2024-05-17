LRESULT HWNDMessageHandler::OnNCUAHDrawCaption(UINT message,
                                               WPARAM w_param,
                                               LPARAM l_param) {
  SetMsgHandled(delegate_->IsUsingCustomFrame());
  return 0;
}
