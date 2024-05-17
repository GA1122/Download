void ChildProcessSecurityPolicyImpl::AddIsolatedOrigins(
    std::vector<url::Origin> origins_to_add) {
  base::EraseIf(origins_to_add, [](const url::Origin& origin) {
    if (IsolatedOriginUtil::IsValidIsolatedOrigin(origin))
      return false;   

    LOG(ERROR) << "Invalid isolated origin: " << origin;
    return true;   
  });

  base::AutoLock lock(lock_);
  for (const url::Origin& origin : origins_to_add) {
    GURL key(SiteInstanceImpl::GetSiteForOrigin(origin));
    isolated_origins_[key].insert(origin);
  }
}
