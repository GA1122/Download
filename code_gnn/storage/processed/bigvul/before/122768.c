void BrowserPluginGuest::SetFocus(bool focused) {
  if (focused_ == focused)
      return;
  focused_ = focused;
  Send(new ViewMsg_SetFocus(routing_id(), focused));
}
