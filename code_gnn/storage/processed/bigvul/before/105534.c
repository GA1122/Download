bool SendCaptureEntirePageJSONRequest(
    AutomationMessageSender* sender,
    int browser_index,
    int tab_index,
    const FilePath& path,
    std::string* error_msg) {
  DictionaryValue dict;
  dict.SetString("command", "CaptureEntirePage");
  dict.SetInteger("windex", browser_index);
  dict.SetInteger("tab_index", tab_index);
  dict.SetString("path", path.value());
  DictionaryValue reply_dict;

  return SendAutomationJSONRequest(sender, dict, &reply_dict, error_msg);
}
