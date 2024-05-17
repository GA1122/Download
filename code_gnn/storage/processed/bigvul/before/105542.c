bool SendGetCookiesJSONRequestDeprecated(
    AutomationMessageSender* sender,
    int browser_index,
    const std::string& url,
    std::string* cookies) {
  DictionaryValue dict;
  dict.SetString("command", "GetCookies");
  dict.SetInteger("windex", browser_index);
  dict.SetString("url", url);
  DictionaryValue reply_dict;
  std::string error_msg;
  if (!SendAutomationJSONRequest(sender, dict, &reply_dict, &error_msg))
    return false;
  return reply_dict.GetString("cookies", cookies);
}
