void TestingAutomationProvider::SendSuccessReply(IPC::Message* reply_message) {
  AutomationJSONReply(this, reply_message).SendSuccess(NULL);
}
