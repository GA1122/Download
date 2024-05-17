LRESULT HWNDMessageHandler::OnImeMessages(UINT message,
                                          WPARAM w_param,
                                          LPARAM l_param) {
  LRESULT result = 0;
  SetMsgHandled(delegate_->HandleIMEMessage(
      message, w_param, l_param, &result));
  return result;
}
