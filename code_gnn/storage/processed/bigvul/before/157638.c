int GetTabId(base::DictionaryValue* tab) {
  int id = kUndefinedId;
  if (tab)
    tab->GetInteger(keys::kIdKey, &id);
  return id;
}
