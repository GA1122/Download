bool PrintWebViewHelper::IsPrintingEnabled() {
  bool result = false;
  Send(new PrintHostMsg_IsPrintingEnabled(routing_id(), &result));
  return result;
}
