void ExtensionPrefs::SetExtensionPrefURLPatternSet(
    const std::string& extension_id,
    const std::string& pref_key,
    const URLPatternSet& new_value) {
  ListValue* value = new ListValue();
  for (URLPatternSet::const_iterator i = new_value.begin();
       i != new_value.end(); ++i)
    value->AppendIfNotPresent(Value::CreateStringValue(i->GetAsString()));

  UpdateExtensionPref(extension_id, pref_key, value);
}
