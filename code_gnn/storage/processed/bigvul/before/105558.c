bool SendNavigateToURLJSONRequest(
    AutomationMessageSender* sender,
    int browser_index,
    int tab_index,
    const GURL& url,
    int navigation_count,
    AutomationMsg_NavigationResponseValues* nav_response,
    std::string* error_msg) {
  DictionaryValue dict;
  dict.SetString("command", "NavigateToURL");
  dict.SetInteger("windex", browser_index);
  dict.SetInteger("tab_index", tab_index);
  dict.SetString("url", url.possibly_invalid_spec());
  dict.SetInteger("navigation_count", navigation_count);
  DictionaryValue reply_dict;
  if (!SendAutomationJSONRequest(sender, dict, &reply_dict, error_msg))
    return false;
  int response = 0;
  if (!reply_dict.GetInteger("result", &response))
    return false;
  *nav_response = static_cast<AutomationMsg_NavigationResponseValues>(response);
  return true;
}
