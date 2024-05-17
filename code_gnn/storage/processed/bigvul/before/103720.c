bool DevToolsAgentFilter::OnMessageReceived(const IPC::Message& message) {
  message_handled_ = true;
  current_routing_id_ = message.routing_id();
  IPC_BEGIN_MESSAGE_MAP(DevToolsAgentFilter, message)
    IPC_MESSAGE_HANDLER(DevToolsAgentMsg_DebuggerCommand, OnDebuggerCommand)
    IPC_MESSAGE_HANDLER(DevToolsAgentMsg_DispatchOnInspectorBackend,
                        OnDispatchOnInspectorBackend)
    IPC_MESSAGE_UNHANDLED(message_handled_ = false)
  IPC_END_MESSAGE_MAP()
  return message_handled_;
}
