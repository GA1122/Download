const URLPatternSet PermissionsData::policy_blocked_hosts() const {
  base::AutoLock auto_lock(runtime_lock_);
  return PolicyBlockedHostsUnsafe();
}
