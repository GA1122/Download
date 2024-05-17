StringOrdinal ExtensionPrefs::GetAppLaunchOrdinal(
    const std::string& extension_id) const {
  std::string raw_value;
  ReadExtensionPrefString(extension_id, kPrefAppLaunchOrdinal, &raw_value);
  return StringOrdinal(raw_value);
}
