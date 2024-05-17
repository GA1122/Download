PermissionMessages PermissionsData::GetPermissionMessages() const {
  base::AutoLock auto_lock(runtime_lock_);
  return PermissionMessageProvider::Get()->GetPermissionMessages(
      PermissionMessageProvider::Get()->GetAllPermissionIDs(
          *active_permissions_unsafe_, manifest_type_));
}
