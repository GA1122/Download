void TestingAutomationProvider::SetProxySettings(Browser* browser,
                                                 DictionaryValue* args,
                                                 IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  std::string key;
  base::Value* value;
  if (!args->GetString("key", &key) || !args->Get("value", &value)) {
    reply.SendError("Invalid or missing args.");
    return;
  }

  std::string setting_path = "cros.session.proxy.";
  setting_path.append(key);

  chromeos::proxy_cros_settings_parser::SetProxyPrefValue(
      browser->profile(), setting_path, value);
  reply.SendSuccess(NULL);
}
