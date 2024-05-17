void TestingAutomationProvider::GetPrefsInfo(DictionaryValue* args,
                                             IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  Browser* browser;
  std::string error_msg;
  if (!GetBrowserFromJSONArgs(args, &browser, &error_msg)) {
    reply.SendError(error_msg);
    return;
  }
  DictionaryValue* items = browser->profile()->GetPrefs()->
      GetPreferenceValues();

  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  return_value->Set("prefs", items);   
  reply.SendSuccess(return_value.get());
}
