bool AutomationMouseEventProcessor::OnMessageReceived(
    const IPC::Message& message) {
  bool handled = true;
  bool msg_is_good = true;
  IPC_BEGIN_MESSAGE_MAP_EX(AutomationMouseEventProcessor, message, msg_is_good)
    IPC_MESSAGE_HANDLER(AutomationMsg_WillProcessMouseEventAt,
                        OnWillProcessMouseEventAt)
    IPC_MESSAGE_HANDLER(AutomationMsg_ProcessMouseEventACK,
                        OnProcessMouseEventACK)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP_EX()
  if (!msg_is_good) {
    LOG(ERROR) << "Failed to deserialize an IPC message";
  }
  return handled;
}
