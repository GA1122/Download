StringOrdinal ExtensionPrefs::CreateFirstAppPageOrdinal() const {
  const DictionaryValue* extensions = prefs_->GetDictionary(kExtensionsPref);
  CHECK(extensions);

  if (page_ordinal_map_.empty())
    return StringOrdinal::CreateInitialOrdinal();

  return page_ordinal_map_.begin()->first;
}
