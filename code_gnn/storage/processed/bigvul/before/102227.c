void ExtensionPrefs::AddGrantedPermissions(
    const std::string& extension_id,
    const ExtensionPermissionSet* permissions) {
  CHECK(Extension::IdIsValid(extension_id));

  scoped_refptr<ExtensionPermissionSet> granted_permissions(
      GetGrantedPermissions(extension_id));

  scoped_refptr<ExtensionPermissionSet> new_perms(
      ExtensionPermissionSet::CreateUnion(
          permissions, granted_permissions.get()));

  SetExtensionPrefPermissionSet(
      extension_id, kPrefGrantedPermissions, new_perms.get());
}
