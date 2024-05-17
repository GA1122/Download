const URLPatternSet& PermissionsData::PolicyAllowedHostsUnsafe() const {
  runtime_lock_.AssertAcquired();
  if (uses_default_policy_host_restrictions)
    return default_policy_allowed_hosts();
  return policy_allowed_hosts_unsafe_;
}
