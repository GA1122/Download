 RenderProcessHost* RenderProcessHostImpl::GetProcessHostForSite(
     BrowserContext* browser_context,
     const GURL& url) {
    SiteProcessMap* map =
        GetSiteProcessMapForBrowserContext(browser_context);
  
//    
//    
    std::string site = SiteInstance::GetSiteForURL(browser_context, url)
        .possibly_invalid_spec();
  return map->FindProcess(site);
//   RenderProcessHost* host = map->FindProcess(site);
//   if (host && !IsSuitableHost(host, browser_context, url)) {
//      
//      
//     RecordAction(UserMetricsAction("BindingsMismatch_GetProcessHostPerSite"));
//     map->RemoveProcess(host);
//     host = NULL;
//   }
// 
//   return host;
  }