bool OneClickSigninHelper::OnMessageReceived(const IPC::Message& message) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(OneClickSigninHelper, message)
    IPC_MESSAGE_HANDLER(OneClickSigninHostMsg_FormSubmitted, OnFormSubmitted)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  return handled;
}
