bool Extension::CheckAPIPermissionWithParam(APIPermission::ID permission,
    const APIPermission::CheckParam* param) const {
  base::AutoLock auto_lock(runtime_data_lock_);
  return runtime_data_.GetActivePermissions()->
      CheckAPIPermissionWithParam(permission, param);
}
