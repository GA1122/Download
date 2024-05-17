void ChildProcessSecurityPolicy::GrantRequestURL(
    int renderer_id, const GURL& url) {

  if (!url.is_valid())
    return;   

  if (IsWebSafeScheme(url.scheme()))
    return;   

  if (IsPseudoScheme(url.scheme())) {
    if (url.SchemeIs(chrome::kViewSourceScheme) ||
        url.SchemeIs(chrome::kPrintScheme)) {
      GrantRequestURL(renderer_id, GURL(url.path()));
    }

    return;   
  }

  {
    AutoLock lock(lock_);
    SecurityStateMap::iterator state = security_state_.find(renderer_id);
    if (state == security_state_.end())
      return;

    state->second->GrantScheme(url.scheme());
  }
}
