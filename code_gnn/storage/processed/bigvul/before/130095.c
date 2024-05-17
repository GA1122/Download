bool MasterPreferences::GetExtensionsBlock(
    base::DictionaryValue** extensions) const {
  return master_dictionary_->GetDictionary(
      master_preferences::kExtensionsBlock, extensions);
 }
