int ExtensionWindowLastFocusedTest::GetTabId(
    const base::DictionaryValue* value) const {
  const base::ListValue* tabs = NULL;
  if (!value->GetList(keys::kTabsKey, &tabs))
    return -2;
  const base::Value* tab = NULL;
  if (!tabs->Get(0, &tab))
    return -2;
  const base::DictionaryValue* tab_dict = NULL;
  if (!tab->GetAsDictionary(&tab_dict))
    return -2;
  int tab_id = 0;
  if (!tab_dict->GetInteger(keys::kIdKey, &tab_id))
    return -2;
  return tab_id;
}
