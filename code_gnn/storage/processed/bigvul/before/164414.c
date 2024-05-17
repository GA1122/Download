bool PermissionsData::HasAPIPermission(
    const std::string& permission_name) const {
  base::AutoLock auto_lock(runtime_lock_);
  return active_permissions_unsafe_->HasAPIPermission(permission_name);
}
