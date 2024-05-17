const URLPatternSet PermissionsData::policy_allowed_hosts() const {
  base::AutoLock auto_lock(runtime_lock_);
  return PolicyAllowedHostsUnsafe();
}
