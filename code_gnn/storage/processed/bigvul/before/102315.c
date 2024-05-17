void ExtensionPrefs::SetActivePermissions(
    const std::string& extension_id,
    const ExtensionPermissionSet* permissions) {
  SetExtensionPrefPermissionSet(
      extension_id, kPrefActivePermissions, permissions);
}
