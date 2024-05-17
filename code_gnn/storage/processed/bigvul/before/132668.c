void BlinkTestRunner::SetAcceptAllCookies(bool accept) {
  Send(new LayoutTestHostMsg_AcceptAllCookies(routing_id(), accept));
}
