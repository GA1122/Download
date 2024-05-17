void PermissionsData::SetPermissions(
    std::unique_ptr<const PermissionSet> active,
    std::unique_ptr<const PermissionSet> withheld) const {
  AutoLockOnValidThread lock(runtime_lock_, thread_checker_.get());
  active_permissions_unsafe_ = std::move(active);
  withheld_permissions_unsafe_ = std::move(withheld);
}
