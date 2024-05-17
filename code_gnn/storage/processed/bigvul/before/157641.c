int GetWindowId(base::DictionaryValue* window) {
  int id = kUndefinedId;
  if (window)
    window->GetInteger(keys::kIdKey, &id);
  return id;
}
