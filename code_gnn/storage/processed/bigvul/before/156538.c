GURL ChildProcessSecurityPolicyImpl::GetOriginLock(int child_id) {
  base::AutoLock lock(lock_);
  SecurityStateMap::iterator state = security_state_.find(child_id);
  if (state == security_state_.end())
    return GURL();
  return state->second->origin_lock();
}
