bool PermissionsData::UsesDefaultPolicyHostRestrictions() const {
  DCHECK(!thread_checker_ || thread_checker_->CalledOnValidThread());
  return uses_default_policy_host_restrictions;
}
