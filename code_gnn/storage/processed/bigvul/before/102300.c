bool ExtensionPrefs::ReadBooleanFromPref(
    const DictionaryValue* ext, const std::string& pref_key) {
  bool bool_value = false;
  ext->GetBoolean(pref_key, &bool_value);
  return bool_value;
}
