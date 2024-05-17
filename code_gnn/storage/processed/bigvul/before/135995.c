bool ChildProcessSecurityPolicyImpl::CanCommitURL(int child_id,
                                                  const GURL& url) {
  if (!url.is_valid())
    return false;   

  if (IsPseudoScheme(url.scheme()))
    return base::LowerCaseEqualsASCII(url.spec(), url::kAboutBlankURL);

  if (IsWebSafeScheme(url.scheme()))
    return true;   

  {
    base::AutoLock lock(lock_);

    SecurityStateMap::iterator state = security_state_.find(child_id);
    if (state == security_state_.end())
      return false;

    return state->second->CanCommitURL(url);
  }
}
