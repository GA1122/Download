void BlinkTestRunner::SetDatabaseQuota(int quota) {
  Send(new LayoutTestHostMsg_SetDatabaseQuota(routing_id(), quota));
}
