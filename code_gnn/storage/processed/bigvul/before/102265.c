StringOrdinal ExtensionPrefs::GetPageOrdinal(const std::string& extension_id)
    const {
  std::string raw_data;
  ReadExtensionPrefString(extension_id, kPrefPageOrdinal, &raw_data);
  return StringOrdinal(raw_data);
}
