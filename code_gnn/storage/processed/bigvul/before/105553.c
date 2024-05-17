bool SendMouseClickJSONRequest(
    AutomationMessageSender* sender,
    int browser_index,
    int tab_index,
    automation::MouseButton button,
    int x,
    int y,
    std::string* error_msg) {
  DictionaryValue dict;
  dict.SetString("command", "WebkitMouseClick");
  dict.SetInteger("windex", browser_index);
  dict.SetInteger("tab_index", tab_index);
  dict.SetInteger("button", button);
  dict.SetInteger("x", x);
  dict.SetInteger("y", y);
  DictionaryValue reply_dict;
  return SendAutomationJSONRequest(sender, dict, &reply_dict, error_msg);
}
