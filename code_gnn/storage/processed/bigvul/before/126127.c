void BrowserEventRouter::TabInsertedAt(WebContents* contents,
                                       int index,
                                       bool active) {
  int tab_id = ExtensionTabUtil::GetTabId(contents);
  if (!GetTabEntry(contents)) {
    tab_entries_[tab_id] = TabEntry();

    TabCreatedAt(contents, index, active);
    return;
  }

  scoped_ptr<ListValue> args(new ListValue());
  args->Append(Value::CreateIntegerValue(tab_id));

  DictionaryValue* object_args = new DictionaryValue();
  object_args->Set(tab_keys::kNewWindowIdKey, Value::CreateIntegerValue(
      ExtensionTabUtil::GetWindowIdOfTab(contents)));
  object_args->Set(tab_keys::kNewPositionKey, Value::CreateIntegerValue(
      index));
  args->Append(object_args);

  Profile* profile = Profile::FromBrowserContext(contents->GetBrowserContext());
  DispatchEvent(profile, events::kOnTabAttached, args.Pass(),
                 EventRouter::USER_GESTURE_UNKNOWN);
 }
