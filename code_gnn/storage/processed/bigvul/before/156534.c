ChildProcessSecurityPolicyImpl::CheckOriginLock(int child_id,
                                                const GURL& site_url) {
  base::AutoLock lock(lock_);
  SecurityStateMap::iterator state = security_state_.find(child_id);
  if (state == security_state_.end())
    return ChildProcessSecurityPolicyImpl::CheckOriginLockResult::NO_LOCK;
  return state->second->CheckOriginLock(site_url);
}
