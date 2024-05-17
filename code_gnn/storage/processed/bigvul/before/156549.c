void ChildProcessSecurityPolicyImpl::GrantWebUIBindings(int child_id,
                                                        int bindings) {
  CHECK(bindings & kWebUIBindingsPolicyMask);
  CHECK_EQ(0, bindings & ~kWebUIBindingsPolicyMask);

  base::AutoLock lock(lock_);

  SecurityStateMap::iterator state = security_state_.find(child_id);
  if (state == security_state_.end())
    return;

  state->second->GrantBindings(bindings);

  state->second->GrantRequestScheme(kChromeUIScheme);

  state->second->GrantRequestScheme(url::kFileScheme);
}
