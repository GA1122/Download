bool SocketStreamDispatcherHost::OnMessageReceived(const IPC::Message& message,
                                                   bool* message_was_ok) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP_EX(SocketStreamDispatcherHost, message, *message_was_ok)
    IPC_MESSAGE_HANDLER(SocketStreamHostMsg_Connect, OnConnect)
    IPC_MESSAGE_HANDLER(SocketStreamHostMsg_SendData, OnSendData)
    IPC_MESSAGE_HANDLER(SocketStreamHostMsg_Close, OnCloseReq)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP_EX()
  return handled;
}
