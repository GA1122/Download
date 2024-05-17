void ChildProcessSecurityPolicy::Remove(int renderer_id) {
  AutoLock lock(lock_);
  if (!security_state_.count(renderer_id))
    return;   

  delete security_state_[renderer_id];
  security_state_.erase(renderer_id);
}
