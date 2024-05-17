  static RenderProcessHost* MatchWithSite(BrowserContext* browser_context,
                                          const GURL& site_url) {
    if (!ShouldFindReusableProcessHostForSite(browser_context, site_url))
      return nullptr;

    UnmatchedServiceWorkerProcessTracker* tracker =
        static_cast<UnmatchedServiceWorkerProcessTracker*>(
            browser_context->GetUserData(
                kUnmatchedServiceWorkerProcessTrackerKey));
    if (!tracker)
      return nullptr;
    return tracker->TakeFreshestProcessForSite(site_url);
  }
