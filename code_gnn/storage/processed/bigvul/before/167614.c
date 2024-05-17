bool SiteInstanceImpl::RequiresDedicatedProcess() {
  if (!has_site_)
    return false;

  return DoesSiteRequireDedicatedProcess(GetBrowserContext(), site_);
}
