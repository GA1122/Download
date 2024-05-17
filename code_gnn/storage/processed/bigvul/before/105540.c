bool SendGetChromeDriverAutomationVersion(
    AutomationMessageSender* sender,
    int* version,
    std::string* error_msg) {
  DictionaryValue dict;
  dict.SetString("command", "GetChromeDriverAutomationVersion");
  DictionaryValue reply_dict;
  if (!SendAutomationJSONRequest(sender, dict, &reply_dict, error_msg))
    return false;
  return reply_dict.GetInteger("version", version);
}
