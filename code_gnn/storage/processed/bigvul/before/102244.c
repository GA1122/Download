  virtual DictionaryValue* Get() {
    DictionaryValue* dict = DictionaryPrefUpdate::Get();
    DictionaryValue* preferences = NULL;
    std::string key =
        extension_id_ + std::string(".") + incognito_or_regular_path_;
    if (!dict->GetDictionary(key, &preferences)) {
      preferences = new DictionaryValue;
      dict->Set(key, preferences);
    }
    return preferences;
  }
