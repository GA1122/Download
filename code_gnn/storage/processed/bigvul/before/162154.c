  RenderProcessHost* TakeFreshestProcessForSite(const GURL& site_url) {
    RenderProcessHost* host = FindFreshestProcessForSite(site_url);
    if (!host)
      return nullptr;

    if (!host->MayReuseHost() || !RenderProcessHostImpl::IsSuitableHost(
                                     host, host->GetBrowserContext(), site_url))
      return nullptr;

    site_process_set_.erase(SiteProcessIDPair(site_url, host->GetID()));
    if (!HasProcess(host))
      host->RemoveObserver(this);
    return host;
  }
