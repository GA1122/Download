void ChildProcessSecurityPolicyImpl::GrantCommitOrigin(
    int child_id,
    const url::Origin& origin) {
  base::AutoLock lock(lock_);

  SecurityStateMap::iterator state = security_state_.find(child_id);
  if (state == security_state_.end())
    return;

  state->second->GrantCommitOrigin(origin);
}
