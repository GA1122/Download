void PermissionsData::SetUsesDefaultHostRestrictions() const {
  AutoLockOnValidThread lock(runtime_lock_, thread_checker_.get());
  uses_default_policy_host_restrictions = true;
}
