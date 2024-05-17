void TestingAutomationProvider::GetProxySettings(Browser* browser,
                                                 DictionaryValue* args,
                                                 IPC::Message* reply_message) {
  const char* settings[] = { "pacurl", "singlehttp", "singlehttpport",
                             "httpurl", "httpport", "httpsurl", "httpsport",
                             "type", "single", "ftpurl", "ftpport",
                             "socks", "socksport", "ignorelist" };

  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);

  for (size_t i = 0; i < arraysize(settings); ++i) {
    base::Value* setting = GetProxySetting(browser, settings[i]);
    if (setting)
      return_value->Set(settings[i], setting);
  }

  AutomationJSONReply(this, reply_message).SendSuccess(return_value.get());
}
