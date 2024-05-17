void PermissionsData::SetActivePermissions(
    std::unique_ptr<const PermissionSet> active) const {
  AutoLockOnValidThread lock(runtime_lock_, thread_checker_.get());
  active_permissions_unsafe_ = std::move(active);
}
