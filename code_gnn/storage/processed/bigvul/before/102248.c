std::string ExtensionPrefs::GetAppNotificationClientId(
    const std::string& extension_id) const {
  const DictionaryValue* dict = GetExtensionPref(extension_id);
  if (!dict || !dict->HasKey(kPrefAppNotificationClientId))
    return std::string();

  std::string tmp;
  dict->GetString(kPrefAppNotificationClientId, &tmp);
  return tmp;
}
