bool PermissionsData::HasWithheldImpliedAllHosts() const {
  base::AutoLock auto_lock(runtime_lock_);
  return !withheld_permissions_unsafe_->explicit_hosts().is_empty() ||
         !withheld_permissions_unsafe_->scriptable_hosts().is_empty();
}
