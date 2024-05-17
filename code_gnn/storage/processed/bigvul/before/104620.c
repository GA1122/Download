bool Extension::HasAPIPermission(
    ExtensionAPIPermission::ID permission) const {
  return permission_set()->HasAPIPermission(permission);
}
