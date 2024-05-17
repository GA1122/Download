const URLPatternSet& PermissionsData::default_policy_allowed_hosts() {
  return default_runtime_policy.Get().allowed_hosts;
}
