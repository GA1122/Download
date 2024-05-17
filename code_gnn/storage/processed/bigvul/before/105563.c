bool SendWebKeyEventJSONRequest(
    AutomationMessageSender* sender,
    int browser_index,
    int tab_index,
    const WebKeyEvent& key_event,
    std::string* error_msg) {
  DictionaryValue dict;
  dict.SetString("command", "SendWebkitKeyEvent");
  dict.SetInteger("windex", browser_index);
  dict.SetInteger("tab_index", tab_index);
  dict.SetInteger("type", key_event.type);
  dict.SetInteger("nativeKeyCode", key_event.key_code);
  dict.SetInteger("windowsKeyCode", key_event.key_code);
  dict.SetString("unmodifiedText", key_event.unmodified_text);
  dict.SetString("text", key_event.modified_text);
  dict.SetInteger("modifiers", key_event.modifiers);
  dict.SetBoolean("isSystemKey", false);
  DictionaryValue reply_dict;
  return SendAutomationJSONRequest(sender, dict, &reply_dict, error_msg);
}
