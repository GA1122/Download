bool ExtensionPrefs::IsAppNotificationDisabled(
    const std::string& extension_id) const {
  return ReadExtensionPrefBoolean(extension_id, kPrefAppNotificationDisbaled);
}
