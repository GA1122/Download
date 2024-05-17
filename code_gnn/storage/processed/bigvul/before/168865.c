PermissionsData::PermissionsData(const Extension* extension)
    : extension_id_(extension->id()), manifest_type_(extension->GetType()) {
  const PermissionSet& required_permissions =
      PermissionsParser::GetRequiredPermissions(extension);
  active_permissions_unsafe_.reset(new PermissionSet(
      required_permissions.apis(), required_permissions.manifest_permissions(),
      required_permissions.explicit_hosts(),
      required_permissions.scriptable_hosts()));
  withheld_permissions_unsafe_.reset(new PermissionSet());
}
