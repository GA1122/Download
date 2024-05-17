void PermissionsData::SetDefaultPolicyHostRestrictions(
    const URLPatternSet& default_policy_blocked_hosts,
    const URLPatternSet& default_policy_allowed_hosts) {
  default_policy_restrictions.Get().blocked_hosts =
      default_policy_blocked_hosts;
  default_policy_restrictions.Get().allowed_hosts =
      default_policy_allowed_hosts;
}
