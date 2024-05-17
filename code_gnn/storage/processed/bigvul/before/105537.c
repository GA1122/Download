bool SendDeleteCookieJSONRequestDeprecated(
    AutomationMessageSender* sender,
    int browser_index,
    const std::string& url,
    const std::string& cookie_name) {
  DictionaryValue dict;
  dict.SetString("command", "DeleteCookie");
  dict.SetInteger("windex", browser_index);
  dict.SetString("url", url);
  dict.SetString("name", cookie_name);
  DictionaryValue reply_dict;
  std::string error_msg;
  return SendAutomationJSONRequest(sender, dict, &reply_dict, &error_msg);
}
