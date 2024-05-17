void PermissionsData::ClearTabSpecificPermissions(int tab_id) const {
  AutoLockOnValidThread lock(runtime_lock_, thread_checker_.get());
  CHECK_GE(tab_id, 0);
  tab_specific_permissions_.erase(tab_id);
}
