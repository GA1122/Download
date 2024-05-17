void PermissionsData::ClearTabSpecificPermissions(int tab_id) const {
  base::AutoLock auto_lock(runtime_lock_);
  CHECK_GE(tab_id, 0);
  tab_specific_permissions_.erase(tab_id);
}
