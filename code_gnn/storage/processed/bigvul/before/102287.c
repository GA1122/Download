bool ExtensionPrefs::IsOrphanedExtensionAcknowledged(
    const std::string& extension_id) {
  return ReadExtensionPrefBoolean(extension_id, kPrefOrphanAcknowledged);
}
