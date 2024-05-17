StringOrdinal ExtensionPrefs::GetNaturalAppPageOrdinal() const {
  const DictionaryValue* extensions = prefs_->GetDictionary(kExtensionsPref);
  CHECK(extensions);

  if (page_ordinal_map_.empty())
    return StringOrdinal::CreateInitialOrdinal();

  for (PageOrdinalMap::const_iterator it = page_ordinal_map_.begin();
       it != page_ordinal_map_.end(); ++it) {
    if (it->second < kNaturalAppPageSize)
      return it->first;
  }

  StringOrdinal last_element = page_ordinal_map_.rbegin()->first;
  return last_element.CreateAfter();
}
