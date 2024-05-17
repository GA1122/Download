bool PermissionsData::CheckAPIPermissionWithParam(
    APIPermission::ID permission,
    const APIPermission::CheckParam* param) const {
  return active_permissions()->CheckAPIPermissionWithParam(permission, param);
}
