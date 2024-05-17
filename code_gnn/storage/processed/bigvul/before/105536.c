bool SendDeleteCookieJSONRequest(
    AutomationMessageSender* sender,
    const std::string& url,
    const std::string& cookie_name,
    std::string* error_msg) {
  DictionaryValue dict;
  dict.SetString("command", "DeleteCookie");
  dict.SetString("url", url);
  dict.SetString("name", cookie_name);
  DictionaryValue reply_dict;
  return SendAutomationJSONRequest(sender, dict, &reply_dict, error_msg);
}
