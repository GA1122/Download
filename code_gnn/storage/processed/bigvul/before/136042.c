void ChildProcessSecurityPolicyImpl::Remove(int child_id) {
  base::AutoLock lock(lock_);
  SecurityStateMap::iterator it = security_state_.find(child_id);
  if (it == security_state_.end())
    return;   

  delete it->second;
  security_state_.erase(it);
  worker_map_.erase(child_id);
}
