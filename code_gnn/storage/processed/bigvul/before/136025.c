void ChildProcessSecurityPolicyImpl::GrantRequestSpecificFileURL(
    int child_id,
    const GURL& url) {
  if (!url.SchemeIs(url::kFileScheme))
    return;

  {
    base::AutoLock lock(lock_);
    SecurityStateMap::iterator state = security_state_.find(child_id);
    if (state == security_state_.end())
      return;

    base::FilePath path;
    if (net::FileURLToFilePath(url, &path))
      state->second->GrantRequestOfSpecificFile(path);
  }
}
