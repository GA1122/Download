void PermissionsData::SetPolicyHostRestrictions(
    const URLPatternSet& runtime_blocked_hosts,
    const URLPatternSet& runtime_allowed_hosts) const {
  AutoLockOnValidThread lock(runtime_lock_, thread_checker_.get());
  policy_blocked_hosts_unsafe_ = runtime_blocked_hosts;
  policy_allowed_hosts_unsafe_ = runtime_allowed_hosts;
  uses_default_policy_host_restrictions = false;
}
