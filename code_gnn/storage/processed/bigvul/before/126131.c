void BrowserEventRouter::TabSelectionChanged(
    TabStripModel* tab_strip_model,
    const TabStripSelectionModel& old_model) {
  TabStripSelectionModel::SelectedIndices new_selection =
      tab_strip_model->selection_model().selected_indices();
  ListValue* all = new ListValue();

  for (size_t i = 0; i < new_selection.size(); ++i) {
    int index = new_selection[i];
    WebContents* contents = tab_strip_model->GetWebContentsAt(index);
    if (!contents)
      break;
    int tab_id = ExtensionTabUtil::GetTabId(contents);
    all->Append(Value::CreateIntegerValue(tab_id));
  }

  scoped_ptr<ListValue> args(new ListValue());
  DictionaryValue* select_info = new DictionaryValue();

  select_info->Set(tab_keys::kWindowIdKey, Value::CreateIntegerValue(
      ExtensionTabUtil::GetWindowIdOfTabStripModel(tab_strip_model)));

  select_info->Set(tab_keys::kTabIdsKey, all);
  args->Append(select_info);

  Profile* profile = tab_strip_model->profile();
  DispatchEvent(profile, events::kOnTabHighlightChanged,
                scoped_ptr<ListValue>(args->DeepCopy()),
                EventRouter::USER_GESTURE_UNKNOWN);
  DispatchEvent(profile, events::kOnTabHighlighted, args.Pass(),
                EventRouter::USER_GESTURE_UNKNOWN);
}
