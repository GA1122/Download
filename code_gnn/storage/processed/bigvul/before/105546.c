bool SendGetTabTitleJSONRequest(
    AutomationMessageSender* sender,
    int browser_index,
    int tab_index,
    std::string* tab_title,
    std::string* error_msg) {
  DictionaryValue dict;
  dict.SetString("command", "GetTabTitle");
  dict.SetInteger("windex", browser_index);
  dict.SetInteger("tab_index", tab_index);
  DictionaryValue reply_dict;
  if (!SendAutomationJSONRequest(sender, dict, &reply_dict, error_msg))
    return false;
  return reply_dict.GetString("title", tab_title);
}
