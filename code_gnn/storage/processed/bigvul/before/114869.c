void TestingAutomationProvider::DeleteCookieJSON(
    DictionaryValue* args, IPC::Message* reply_message) {
  automation_util::DeleteCookieJSON(this, args, reply_message);
}
