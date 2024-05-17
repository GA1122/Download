void BlinkTestRunner::ClearAllDatabases() {
  Send(new LayoutTestHostMsg_ClearAllDatabases(routing_id()));
}
