void BrowserEventRouter::TabReplacedAt(TabStripModel* tab_strip_model,
                                       WebContents* old_contents,
                                       WebContents* new_contents,
                                       int index) {
  const int new_tab_id = ExtensionTabUtil::GetTabId(new_contents);
  const int old_tab_id = ExtensionTabUtil::GetTabId(old_contents);
  scoped_ptr<ListValue> args(new ListValue());
  args->Append(Value::CreateIntegerValue(new_tab_id));
  args->Append(Value::CreateIntegerValue(old_tab_id));

  DispatchEvent(Profile::FromBrowserContext(new_contents->GetBrowserContext()),
                events::kOnTabReplaced,
                args.Pass(),
                EventRouter::USER_GESTURE_UNKNOWN);

  const int removed_count = tab_entries_.erase(old_tab_id);
  DCHECK_GT(removed_count, 0);
  UnregisterForTabNotifications(old_contents);

  if (!GetTabEntry(new_contents)) {
    tab_entries_[new_tab_id] = TabEntry();
    RegisterForTabNotifications(new_contents);
  }
}
