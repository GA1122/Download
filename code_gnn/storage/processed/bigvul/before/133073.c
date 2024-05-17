void HWNDMessageHandler::DispatchKeyEventPostIME(const ui::KeyEvent& key) {
  SetMsgHandled(delegate_->HandleKeyEvent(key));
}
