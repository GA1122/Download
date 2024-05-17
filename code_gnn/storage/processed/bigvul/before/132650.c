bool BlinkTestRunner::OnMessageReceived(const IPC::Message& message) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(BlinkTestRunner, message)
    IPC_MESSAGE_HANDLER(ShellViewMsg_SetTestConfiguration,
                        OnSetTestConfiguration)
    IPC_MESSAGE_HANDLER(ShellViewMsg_SessionHistory, OnSessionHistory)
    IPC_MESSAGE_HANDLER(ShellViewMsg_Reset, OnReset)
    IPC_MESSAGE_HANDLER(ShellViewMsg_NotifyDone, OnNotifyDone)
    IPC_MESSAGE_HANDLER(ShellViewMsg_TryLeakDetection, OnTryLeakDetection)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  return handled;
}
