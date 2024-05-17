bool ExtensionPrefs::ReadExtensionPrefInteger(
    const std::string& extension_id, const std::string& pref_key,
    int* out_value) {
  const DictionaryValue* ext = GetExtensionPref(extension_id);
  if (!ext) {
    return false;
  }
  return ReadIntegerFromPref(ext, pref_key, out_value);
}
