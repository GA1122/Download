  virtual DictionaryValue* Get() {
    DictionaryValue* dict = DictionaryPrefUpdate::Get();
    DictionaryValue* extension = NULL;
    if (!dict->GetDictionary(extension_id_, &extension)) {
      extension = new DictionaryValue();
      dict->Set(extension_id_, extension);
    }
    return extension;
  }
