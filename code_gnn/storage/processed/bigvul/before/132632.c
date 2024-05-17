void BlinkTestRunner::DeleteAllCookies() {
  Send(new LayoutTestHostMsg_DeleteAllCookies(routing_id()));
}
