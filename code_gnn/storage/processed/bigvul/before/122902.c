void RenderProcessHostImpl::RegisterProcessHostForSite(
    BrowserContext* browser_context,
    RenderProcessHost* process,
    const GURL& url) {
  SiteProcessMap* map =
      GetSiteProcessMapForBrowserContext(browser_context);

  std::string site = SiteInstanceImpl::GetSiteForURL(browser_context, url)
      .possibly_invalid_spec();
  map->RegisterProcess(site, process);
}
