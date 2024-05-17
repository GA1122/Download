void BlinkTestRunner::ClearDevToolsLocalStorage() {
  Send(new ShellViewHostMsg_ClearDevToolsLocalStorage(routing_id()));
}
