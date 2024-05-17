int GetTabWindowId(base::DictionaryValue* tab) {
  int id = kUndefinedId;
  if (tab)
    tab->GetInteger(keys::kWindowIdKey, &id);
  return id;
}
