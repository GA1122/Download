const URLPatternSet& PermissionsData::GetEffectiveHostPermissions() const {
  return active_permissions()->effective_hosts();
}
