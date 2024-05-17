void TestingAutomationProvider::GetCookiesJSON(
    DictionaryValue* args, IPC::Message* reply_message) {
  automation_util::GetCookiesJSON(this, args, reply_message);
}
