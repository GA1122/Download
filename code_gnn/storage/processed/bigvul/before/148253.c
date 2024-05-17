bool PepperRendererConnection::OnMessageReceived(const IPC::Message& msg) {
  if (in_process_host_->GetPpapiHost()->OnMessageReceived(msg))
    return true;

  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(PepperRendererConnection, msg)
    IPC_MESSAGE_HANDLER(PpapiHostMsg_CreateResourceHostsFromHost,
                        OnMsgCreateResourceHostsFromHost)
    IPC_MESSAGE_HANDLER(FrameHostMsg_DidCreateInProcessInstance,
                        OnMsgDidCreateInProcessInstance)
    IPC_MESSAGE_HANDLER(FrameHostMsg_DidDeleteInProcessInstance,
                        OnMsgDidDeleteInProcessInstance)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  return handled;
}
