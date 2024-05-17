bool ExtensionPrefs::ReadExtensionPrefString(
    const std::string& extension_id, const std::string& pref_key,
    std::string* out_value) const {
  const DictionaryValue* ext = GetExtensionPref(extension_id);

  if (!ext || !ext->GetString(pref_key, out_value))
    return false;

  return true;
}
