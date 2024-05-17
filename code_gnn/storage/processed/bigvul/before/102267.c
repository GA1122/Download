std::string ExtensionPrefs::GetUpdateUrlData(const std::string& extension_id) {
  const DictionaryValue* dictionary = GetExtensionPref(extension_id);
  if (!dictionary)
    return std::string();

  std::string data;
  dictionary->GetString(kUpdateUrlData, &data);
  return data;
}
