bool SiteInstanceImpl::IsOriginLockASite(const GURL& lock_url) {
  return lock_url.has_scheme() && lock_url.has_host();
}
