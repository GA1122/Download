void BlinkTestRunner::ResetPermissions() {
  Send(new LayoutTestHostMsg_ResetPermissions(routing_id()));
}
