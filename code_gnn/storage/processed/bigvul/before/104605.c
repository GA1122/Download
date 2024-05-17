const URLPatternSet& Extension::GetEffectiveHostPermissions() const {
  return permission_set()->effective_hosts();
}
