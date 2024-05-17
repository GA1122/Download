void ChildProcessSecurityPolicyImpl::GrantRequestURL(
    int child_id, const GURL& url) {

  if (!url.is_valid())
    return;   

  if (IsWebSafeScheme(url.scheme()))
    return;   

  if (IsPseudoScheme(url.scheme())) {
    if (url.SchemeIs(chrome::kViewSourceScheme)) {
      GrantRequestURL(child_id, GURL(url.path()));
    }

    return;   
  }

  {
    base::AutoLock lock(lock_);
    SecurityStateMap::iterator state = security_state_.find(child_id);
    if (state == security_state_.end())
      return;

    state->second->GrantScheme(url.scheme());
  }
}
