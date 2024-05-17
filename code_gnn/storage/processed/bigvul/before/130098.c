std::vector<std::string> GetNamedList(const char* name,
                                      const base::DictionaryValue* prefs) {
  std::vector<std::string> list;
  if (!prefs)
    return list;

  const base::ListValue* value_list = NULL;
  if (!prefs->GetList(name, &value_list))
    return list;

  list.reserve(value_list->GetSize());
  for (size_t i = 0; i < value_list->GetSize(); ++i) {
    const base::Value* entry;
    std::string url_entry;
    if (!value_list->Get(i, &entry) || !GetURLFromValue(entry, &url_entry)) {
      NOTREACHED();
      break;
    }
    list.push_back(url_entry);
  }
  return list;
}
