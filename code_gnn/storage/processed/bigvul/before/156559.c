void ChildProcessSecurityPolicyImpl::RemoveIsolatedOriginForTesting(
    const url::Origin& origin) {
  GURL key(SiteInstanceImpl::GetSiteForOrigin(origin));
  base::AutoLock lock(lock_);
  isolated_origins_[key].erase(origin);
  if (isolated_origins_[key].empty())
    isolated_origins_.erase(key);
}
