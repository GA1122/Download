bool Extension::LoadManagedModeFeatures(string16* error) {
  if (!manifest_->HasKey(keys::kContentPack))
    return true;
  DictionaryValue* content_pack_value = NULL;
  if (!manifest_->GetDictionary(keys::kContentPack, &content_pack_value)) {
    *error = ASCIIToUTF16(errors::kInvalidContentPack);
    return false;
  }

  if (!LoadManagedModeSites(content_pack_value, error))
    return false;
  if (!LoadManagedModeConfigurations(content_pack_value, error))
    return false;

  return true;
}
