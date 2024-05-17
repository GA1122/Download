const URLPatternSet& PermissionsData::default_policy_blocked_hosts() {
  return default_runtime_policy.Get().blocked_hosts;
}
