bool SendSetCookieJSONRequest(
    AutomationMessageSender* sender,
    const std::string& url,
    DictionaryValue* cookie_dict,
    std::string* error_msg) {
  DictionaryValue dict;
  dict.SetString("command", "SetCookie");
  dict.SetString("url", url);
  dict.Set("cookie", cookie_dict->DeepCopy());
  DictionaryValue reply_dict;
  return SendAutomationJSONRequest(sender, dict, &reply_dict, error_msg);
}
