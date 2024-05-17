void TestingAutomationProvider::SetCookieJSON(
    DictionaryValue* args, IPC::Message* reply_message) {
  automation_util::SetCookieJSON(this, args, reply_message);
}
