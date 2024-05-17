const URLPatternSet& PermissionsData::PolicyBlockedHostsUnsafe() const {
  runtime_lock_.AssertAcquired();
  if (uses_default_policy_host_restrictions)
    return default_policy_blocked_hosts();
  return policy_blocked_hosts_unsafe_;
}
