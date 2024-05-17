bool SendWaitForAllTabsToStopLoadingJSONRequest(
    AutomationMessageSender* sender,
    std::string* error_msg) {
  DictionaryValue dict;
  dict.SetString("command", "WaitForAllTabsToStopLoading");
  DictionaryValue reply_dict;
  return SendAutomationJSONRequest(sender, dict, &reply_dict, error_msg);
}
