bool SendSetCookieJSONRequestDeprecated(
    AutomationMessageSender* sender,
    int browser_index,
    const std::string& url,
    const std::string& cookie) {
  DictionaryValue dict;
  dict.SetString("command", "SetCookie");
  dict.SetInteger("windex", browser_index);
  dict.SetString("url", url);
  dict.SetString("cookie", cookie);
  DictionaryValue reply_dict;
  std::string error_msg;
  return SendAutomationJSONRequest(sender, dict, &reply_dict, &error_msg);
}
