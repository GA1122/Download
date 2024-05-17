void TestingAutomationProvider::GetLocalStatePrefsInfo(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  DictionaryValue* items = g_browser_process->local_state()->
      GetPreferenceValues();
  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  return_value->Set("prefs", items);   
  AutomationJSONReply(this, reply_message).SendSuccess(return_value.get());
}
