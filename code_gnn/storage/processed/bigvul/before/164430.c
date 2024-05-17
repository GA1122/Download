const URLPatternSet& PermissionsData::default_policy_allowed_hosts() {
  return default_policy_restrictions.Get().allowed_hosts;
}
