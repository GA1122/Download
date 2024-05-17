bool SendAutomationJSONRequest(AutomationMessageSender* sender,
                               const std::string& request,
                               int timeout_ms,
                               std::string* reply,
                               bool* success) {
  return sender->Send(new AutomationMsg_SendJSONRequest(
       -1, request, reply, success), timeout_ms);
 }
