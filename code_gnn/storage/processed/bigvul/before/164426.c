void PermissionsData::SetPolicyHostRestrictions(
    const URLPatternSet& policy_blocked_hosts,
    const URLPatternSet& policy_allowed_hosts) const {
  AutoLockOnValidThread lock(runtime_lock_, thread_checker_.get());
  policy_blocked_hosts_unsafe_ = policy_blocked_hosts;
  policy_allowed_hosts_unsafe_ = policy_allowed_hosts;
  uses_default_policy_host_restrictions = false;
}
