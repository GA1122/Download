void BlinkTestRunner::Reload() {
  Send(new ShellViewHostMsg_Reload(routing_id()));
}
