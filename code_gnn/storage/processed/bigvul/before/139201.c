void RenderProcessHostImpl::RegisterProcessHostForSite(
    BrowserContext* browser_context,
    RenderProcessHost* process,
    const GURL& url) {
  SiteProcessMap* map = GetSiteProcessMapForBrowserContext(browser_context);

  std::string site =
      SiteInstance::GetSiteForURL(browser_context, url).possibly_invalid_spec();
  if (!site.empty())
    map->RegisterProcess(site, process);
}
