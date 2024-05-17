PepperFlashRendererHost::~PepperFlashRendererHost() {
  std::vector<ppapi::host::ReplyMessageContext>::reverse_iterator it;
  for (it = navigate_replies_.rbegin(); it != navigate_replies_.rend(); ++it)
    SendReply(*it, IPC::Message());
}
