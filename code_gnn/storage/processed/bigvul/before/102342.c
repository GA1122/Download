bool ExtensionPrefs::WasAppDraggedByUser(const std::string& extension_id) {
  return ReadExtensionPrefBoolean(extension_id, kPrefUserDraggedApp);
}
