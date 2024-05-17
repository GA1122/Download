void ChildProcessSecurityPolicyImpl::GrantCommitURL(int child_id,
                                                    const GURL& url) {
  if (!url.is_valid())
    return;

  if (IsPseudoScheme(url.scheme()))
    return;

  url::Origin origin = url::Origin::Create(url);

  if (url.SchemeIsBlob() || url.SchemeIsFileSystem()) {
    if (IsMalformedBlobUrl(url))
      return;

    GrantCommitURL(child_id, GURL(origin.Serialize()));
  }

  if (!origin.unique())
    GrantCommitOrigin(child_id, origin);

  if (IsWebSafeScheme(url.scheme()))
    return;

  base::AutoLock lock(lock_);

  auto state = security_state_.find(child_id);
  if (state == security_state_.end())
    return;

  if (origin.unique()) {
    state->second->GrantCommitScheme(url.scheme());
  } else {
    state->second->GrantRequestScheme(url.scheme());
  }
}
