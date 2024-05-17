bool PermissionsData::CheckAPIPermissionWithParam(
    APIPermission::ID permission,
    const APIPermission::CheckParam* param) const {
  base::AutoLock auto_lock(runtime_lock_);
  return active_permissions_unsafe_->CheckAPIPermissionWithParam(permission,
                                                                 param);
}
