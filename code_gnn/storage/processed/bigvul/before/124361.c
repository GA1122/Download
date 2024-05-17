void BrowserEventRouter::DispatchOldPageActionEvent(
    Profile* profile,
    const std::string& extension_id,
    const std::string& page_action_id,
    int tab_id,
    const std::string& url,
    int button) {
  scoped_ptr<ListValue> args(new ListValue());
  args->Append(Value::CreateStringValue(page_action_id));

  DictionaryValue* data = new DictionaryValue();
  data->Set(tab_keys::kTabIdKey, Value::CreateIntegerValue(tab_id));
  data->Set(tab_keys::kTabUrlKey, Value::CreateStringValue(url));
  data->Set(page_action_keys::kButtonKey, Value::CreateIntegerValue(button));
  args->Append(data);

  DispatchEventToExtension(profile, extension_id, "pageActions", args.Pass(),
                           EventRouter::USER_GESTURE_ENABLED);
}
