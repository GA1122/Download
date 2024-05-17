bool SendIsTabIdValidJSONRequest(
    AutomationMessageSender* sender,
    int tab_id,
    bool* is_valid,
    std::string* error_msg) {
  DictionaryValue dict;
  dict.SetString("command", "IsTabIdValid");
  dict.SetInteger("id", tab_id);
  DictionaryValue reply_dict;
  if (!SendAutomationJSONRequest(sender, dict, &reply_dict, error_msg))
    return false;
  return reply_dict.GetBoolean("is_valid", is_valid);
}
