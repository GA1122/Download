RenderProcessHost* RenderProcessHostImpl::GetProcessHostForSite(
    BrowserContext* browser_context,
    const GURL& url) {
  SiteProcessMap* map = GetSiteProcessMapForBrowserContext(browser_context);

  GURL site_url = SiteInstance::GetSiteForURL(browser_context, url);
  RenderProcessHost* host = map->FindProcess(site_url.possibly_invalid_spec());
  if (host && (!host->MayReuseHost() ||
               !IsSuitableHost(host, browser_context, site_url))) {
    RecordAction(
        base::UserMetricsAction("BindingsMismatch_GetProcessHostPerSite"));
    map->RemoveProcess(host);
    host = nullptr;
  }

  return host;
}
