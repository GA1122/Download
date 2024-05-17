void ChildProcessSecurityPolicyImpl::GrantRequestScheme(
    int child_id,
    const std::string& scheme) {
  base::AutoLock lock(lock_);

  auto state = security_state_.find(child_id);
  if (state == security_state_.end())
    return;

  state->second->GrantRequestScheme(scheme);
}
