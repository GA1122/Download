PermissionMessages PermissionsData::GetNewPermissionMessages(
    const PermissionSet& granted_permissions) const {
  base::AutoLock auto_lock(runtime_lock_);

  std::unique_ptr<const PermissionSet> new_permissions =
      PermissionSet::CreateDifference(*active_permissions_unsafe_,
                                      granted_permissions);

  return PermissionMessageProvider::Get()->GetPermissionMessages(
      PermissionMessageProvider::Get()->GetAllPermissionIDs(*new_permissions,
                                                            manifest_type_));
}
