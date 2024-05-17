void BrowserEventRouter::TabMoved(TabContents* contents,
                                  int from_index,
                                  int to_index) {
  scoped_ptr<ListValue> args(new ListValue());
  args->Append(Value::CreateIntegerValue(
      ExtensionTabUtil::GetTabId(contents->web_contents())));

  DictionaryValue* object_args = new DictionaryValue();
  object_args->Set(tab_keys::kWindowIdKey, Value::CreateIntegerValue(
      ExtensionTabUtil::GetWindowIdOfTab(contents->web_contents())));
  object_args->Set(tab_keys::kFromIndexKey, Value::CreateIntegerValue(
      from_index));
  object_args->Set(tab_keys::kToIndexKey, Value::CreateIntegerValue(
      to_index));
  args->Append(object_args);

  DispatchEvent(contents->profile(), events::kOnTabMoved, args.Pass(),
                EventRouter::USER_GESTURE_UNKNOWN);
}
