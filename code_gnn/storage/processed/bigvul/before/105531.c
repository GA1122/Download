bool SendAcceptOrDismissAppModalDialogJSONRequest(
    AutomationMessageSender* sender,
    bool accept,
    std::string* error_msg) {
  DictionaryValue dict;
  dict.SetString("command", "AcceptOrDismissAppModalDialog");
  dict.SetBoolean("accept", accept);
  DictionaryValue reply_dict;
  return SendAutomationJSONRequest(sender, dict, &reply_dict, error_msg);
}
