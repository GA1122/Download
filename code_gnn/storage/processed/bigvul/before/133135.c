LRESULT HWNDMessageHandler::OnNotify(int w_param, NMHDR* l_param) {
  LRESULT l_result = 0;
  SetMsgHandled(delegate_->HandleTooltipNotify(w_param, l_param, &l_result));
  return l_result;
}
