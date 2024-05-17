void TestingAutomationProvider::UpdateCheck(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  AutomationJSONReply* reply = new AutomationJSONReply(this, reply_message);
  DBusThreadManager::Get()->GetUpdateEngineClient()
      ->RequestUpdateCheck(base::Bind(UpdateCheckCallback, reply));
}
