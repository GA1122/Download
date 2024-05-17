bool PermissionsData::HasAPIPermissionForTab(
    int tab_id,
    APIPermission::ID permission) const {
  base::AutoLock auto_lock(runtime_lock_);
  if (active_permissions_unsafe_->HasAPIPermission(permission))
    return true;

  const PermissionSet* tab_permissions = GetTabSpecificPermissions(tab_id);
  return tab_permissions && tab_permissions->HasAPIPermission(permission);
}
