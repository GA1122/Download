  bool OnMessageReceived(const IPC::Message& message) {
    IPC_BEGIN_MESSAGE_MAP(PluginChannel::MessageFilter, message)
      IPC_MESSAGE_HANDLER_DELAY_REPLY(PluginMsg_Init, OnInit)
      IPC_MESSAGE_HANDLER(PluginMsg_SignalModalDialogEvent,
                          OnSignalModalDialogEvent)
      IPC_MESSAGE_HANDLER(PluginMsg_ResetModalDialogEvent,
                          OnResetModalDialogEvent)
    IPC_END_MESSAGE_MAP()
    return message.type() == PluginMsg_SignalModalDialogEvent::ID ||
           message.type() == PluginMsg_ResetModalDialogEvent::ID;
  }
