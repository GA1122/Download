bool PermissionsData::HasAPIPermission(
    const std::string& permission_name) const {
  return active_permissions()->HasAPIPermission(permission_name);
}
