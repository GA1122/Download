bool ExtensionPrefs::ReadIntegerFromPref(
    const DictionaryValue* ext, const std::string& pref_key, int* out_value) {
  if (!ext->GetInteger(pref_key, out_value))
    return false;

  return out_value != NULL;
}
