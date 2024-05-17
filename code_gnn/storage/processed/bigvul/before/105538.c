bool SendExecuteJavascriptJSONRequest(
    AutomationMessageSender* sender,
    int browser_index,
    int tab_index,
    const std::string& frame_xpath,
    const std::string& javascript,
    Value** result,
    std::string* error_msg) {
  DictionaryValue dict;
  dict.SetString("command", "ExecuteJavascript");
  dict.SetInteger("windex", browser_index);
  dict.SetInteger("tab_index", tab_index);
  dict.SetString("frame_xpath", frame_xpath);
  dict.SetString("javascript", javascript);
  DictionaryValue reply_dict;
  if (!SendAutomationJSONRequest(sender, dict, &reply_dict, error_msg))
    return false;

  std::string json;
  if (!reply_dict.GetString("result", &json)) {
    LOG(ERROR) << "Executed javascript but received no 'result'";
    return false;
  }
  json.insert(0, "[");
  json.append("]");

  JSONStringValueSerializer deserializer(json);
  Value* value = deserializer.Deserialize(NULL, NULL);
  if (!value || !value->IsType(Value::TYPE_LIST)) {
    LOG(ERROR) << "Unable to deserialize returned JSON";
    return false;
  }
  scoped_ptr<ListValue> list(static_cast<ListValue*>(value));
  return list->Remove(0, result);
}
