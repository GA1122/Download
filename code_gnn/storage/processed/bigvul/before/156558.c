void ChildProcessSecurityPolicyImpl::Remove(int child_id) {
  base::AutoLock lock(lock_);
  security_state_.erase(child_id);
  worker_map_.erase(child_id);
}
