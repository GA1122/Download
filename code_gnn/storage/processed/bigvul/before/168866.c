void PermissionsData::SetDefaultPolicyHostRestrictions(
    const URLPatternSet& default_runtime_blocked_hosts,
    const URLPatternSet& default_runtime_allowed_hosts) {
  default_runtime_policy.Get().blocked_hosts = default_runtime_blocked_hosts;
  default_runtime_policy.Get().allowed_hosts = default_runtime_allowed_hosts;
}
