 bool Extension::HasAPIPermissionForTab(int tab_id,
                                       APIPermission::ID permission) const {
  base::AutoLock auto_lock(runtime_data_lock_);
  if (runtime_data_.GetActivePermissions()->HasAPIPermission(permission))
    return true;
  scoped_refptr<const PermissionSet> tab_specific_permissions =
      runtime_data_.GetTabSpecificPermissions(tab_id);
  return tab_specific_permissions.get() &&
         tab_specific_permissions->HasAPIPermission(permission);
}
