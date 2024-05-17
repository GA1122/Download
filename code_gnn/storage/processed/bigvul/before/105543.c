bool SendGetIndicesFromTabHandleJSONRequest(
    AutomationMessageSender* sender,
    int tab_handle,
    int* browser_index,
    int* tab_index,
    std::string* error_msg) {
  DictionaryValue request_dict;
  request_dict.SetString("command", "GetIndicesFromTab");
  request_dict.SetInteger("tab_handle", tab_handle);
  DictionaryValue reply_dict;
  if (!SendAutomationJSONRequest(sender, request_dict, &reply_dict, error_msg))
    return false;
  if (!reply_dict.GetInteger("windex", browser_index))
    return false;
  if (!reply_dict.GetInteger("tab_index", tab_index))
    return false;
  return true;
}