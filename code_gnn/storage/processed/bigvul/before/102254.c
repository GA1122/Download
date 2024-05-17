ExtensionPermissionSet* ExtensionPrefs::GetGrantedPermissions(
    const std::string& extension_id) {
  CHECK(Extension::IdIsValid(extension_id));
  return ReadExtensionPrefPermissionSet(extension_id, kPrefGrantedPermissions);
}
