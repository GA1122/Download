 void PermissionsData::SetActivePermissions(
     const PermissionSet* permissions) const {
   base::AutoLock auto_lock(runtime_lock_);
  active_permissions_unsafe_ = permissions;
}
