void BrowserEventRouter::ActiveTabChanged(WebContents* old_contents,
                                          WebContents* new_contents,
                                          int index,
                                          bool user_gesture) {
  scoped_ptr<ListValue> args(new ListValue());
  int tab_id = ExtensionTabUtil::GetTabId(new_contents);
  args->Append(Value::CreateIntegerValue(tab_id));

  DictionaryValue* object_args = new DictionaryValue();
  object_args->Set(tab_keys::kWindowIdKey, Value::CreateIntegerValue(
      ExtensionTabUtil::GetWindowIdOfTab(new_contents)));
  args->Append(object_args);

  Profile* profile =
      Profile::FromBrowserContext(new_contents->GetBrowserContext());
  EventRouter::UserGestureState gesture = user_gesture ?
      EventRouter::USER_GESTURE_ENABLED : EventRouter::USER_GESTURE_NOT_ENABLED;
  DispatchEvent(profile, events::kOnTabSelectionChanged,
                scoped_ptr<ListValue>(args->DeepCopy()), gesture);
  DispatchEvent(profile, events::kOnTabActiveChanged,
                scoped_ptr<ListValue>(args->DeepCopy()), gesture);

  args->Remove(0, NULL);
  object_args->Set(tab_keys::kTabIdKey, Value::CreateIntegerValue(tab_id));
  DispatchEvent(profile, events::kOnTabActivated, args.Pass(), gesture);
}
